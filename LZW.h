#pragma once
#include<stdlib.h>
#include<string.h>
/*
  STRING TABLE 
  stores a key_string with an Index as value
  key -> index
*/
typedef struct
{
    unsigned int STRING_SIZE;
    unsigned int TABLE_SIZE;
    unsigned int Counter;
    unsigned char * TABLE;
} LZW_table;
/*
  FOR Storing Data after Encoding and Decoding
*/
typedef struct 
{
    unsigned int * STREAM;
    unsigned int MAX_SIZE;
    unsigned int STREAM_SIZE;
} LZW_ENCODED_STREAM;
/*
    Creates a string table with given size;
*/
LZW_table Construct_Table(const int string_size , const int table_size)
{
    LZW_table table;
    table.STRING_SIZE = string_size;
    table.TABLE_SIZE = table_size;
    table.Counter = -1;
    table.TABLE = (unsigned char *)malloc(sizeof(unsigned char) * string_size * table_size);
    return table;
}
/*
  Initializes a table with given Range of characters
*/
void Init_table(LZW_table * const table ,char From , const char To)
{
    table->Counter = To - From;
    int TotalSize = table->TABLE_SIZE * table->STRING_SIZE;
    for( int i = 0 ; i < TotalSize && From <= To ; i += (table->STRING_SIZE) , From++)
    {
        table->TABLE[i] = From;
        table->TABLE[i + 1] = '\0';
    }
}
/*
  Resizes a table  with given size
   if size is lesser then the original table then does nothing (returns 0)
   if greater than increases the size (if not failes allocating return 1 else 0)
*/
int Resize_table(LZW_table * const table , const int size)
{
   if(size > table->TABLE_SIZE)
   {
     if(realloc(table->TABLE , size * table->STRING_SIZE) != NULL)
     {
         table->TABLE_SIZE = size;
         return 1;
     }
     else
     {
        return 0;
     }
   }
   else
   {
       return 0;
   }
}
/*
  Pushes a String in the table
    if table is already full calls Resize on it  (if fails returns 0)
*/
int PushTo_table(LZW_table * const table , const char * const str , const int str_size)
{
    (table->Counter)++;
    if(table->Counter >= table->TABLE_SIZE)
    {
        if(!Resize_table(table,table->TABLE_SIZE + 10))
        {
            return 0;
        }
    }
    unsigned char* ptr = table->TABLE + (table->STRING_SIZE * table->Counter);
    memcpy( ptr , str , str_size);
    memset(ptr + str_size , 0 , table->STRING_SIZE - str_size);
    return 1;
}
/*
  Searches for a string in a table if found returns the code value else returns -1
*/
int SearchIn_Table(LZW_table * const table , const char * const str)
{
    int TotalSize = table->STRING_SIZE * table->Counter;
    for(int i = 0 , j = 0 ; i <= TotalSize ; i += table->STRING_SIZE , j++)
    {
        if(strcmp(str , table->TABLE + i) == 0)
        {   
            return j;
        }
    }
    return -1;
}
/*
  Encodes a given string and returns as a stream
  ON Given ALGO ->
                INITIALIZE: CURRENT = <empty>
                           NEXT     = <1st character in string>
                IF : (CURRENT + NEXT) in STRING_TABLE =>
                        CURRENT = CURRENT + NEXT
                ELSE :
                    ADD CURRENT + NEXT in STRING_TABLE
                    OUTPUT code for CURRENT from STRING_TABLE
                    CURRENT = NEXT
*/
LZW_ENCODED_STREAM LZW_Encode(LZW_table * const table , const char * const String , const int size)
{
    LZW_ENCODED_STREAM LES;
    int streamcounter = -1;
    LES.STREAM = (unsigned int *)calloc(10 , sizeof(unsigned int)); // allocating buffer for the stream
    char CURR[20] = "" , NEXT[1] , CONCATENATED[20];
    for(int i = 0;i < size ; i++)
    {
        NEXT[0] = String[i];
        memcpy(CONCATENATED , CURR , 20); // copying for later use
        strcat(CONCATENATED , NEXT); 
        // CURRENT + NEXT in table ?
        int FoundIndex = SearchIn_Table(table , CONCATENATED);
        if(FoundIndex != -1)
        {
            /*
                Found in table
                 CURRENT = CURRENT + NEXT
            */
            memcpy(CURR , CONCATENATED , 20);
        }
        else
        {
            //Not found   
            PushTo_table(table , CONCATENATED , 20);    //ADD CURRENT + NEXT in table
            LES.STREAM[++streamcounter] = SearchIn_Table(table , CURR);   //OUTPUT code for CURRENT
            CURR[0] = NEXT[0];  //CURRENT = NEXT  (NEXT is always gonna be a single chararcter);
            //Fill empty spaces with null character
            memset(CURR + 1 , 0 , 19); 
            //printf("%d\n" , LES.STREAM[streamcounter]);
        }
    }
    LES.STREAM[++streamcounter] = SearchIn_Table(table , CURR);
    LES.STREAM_SIZE = streamcounter + 1;
    return LES;
} 
void Destruct_Table(LZW_table * table)
{
    table->STRING_SIZE = 0;
    table->TABLE_SIZE = 0;
    free(table->TABLE);
}