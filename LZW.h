#pragma once
#include<stdlib.h>
#include<string.h>
struct LZW_table
{
    int STRING_SIZE;
    int TABLE_SIZE;
    int Counter;
    void * TABLE;
};

struct LZW_table Construct_Table(int string_size , int table_size)
{
    struct LZW_table table;
    table.STRING_SIZE = string_size;
    table.TABLE_SIZE = table_size;
    table.Counter = -1;
    table.TABLE = malloc(sizeof(char) * string_size * table_size);
    return table;
}
void Init_table(struct LZW_table * table)
{
    char * tbl = (char * )table->TABLE;
    for(int i = 0 , j = 0 ; i < table->TABLE_SIZE && j <= 255 ; i += (table->STRING_SIZE) , j++)
    {
        tbl[i] = 'A' + j;
    }
}
int Resize_table(struct LZW_table * table , int size)
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
int PushTo_table(struct LZW_table * table , void * str , int str_size)
{
    (table->Counter)++;
    if(table->Counter >= table->TABLE_SIZE)
    {
        if(!Resize_table(table,table->TABLE_SIZE + 10))
        {
            return 0;
        }
    }
    char* ptr =  (char *)table->TABLE + (table->STRING_SIZE * table->Counter);
    memcpy( ptr , str , str_size);
    memset(ptr + str_size , 0 , table->STRING_SIZE - str_size);
    return 1;
}
void Destruct_Table(struct LZW_table * table)
{
    table->STRING_SIZE = 0;
    table->TABLE_SIZE = 0;
    free(table->TABLE);
}