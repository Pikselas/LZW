#include<stdio.h>
#include"LZW.h"

int main()
{
    LZW_table tab = Construct_Table(20, 400);
    Init_table(&tab , 'A' , 'D');
    LZW_ENCODED_STREAM ls = LZW_Encode(&tab , "ABACABA" , 8);
    for(int i = 0 ; i < ls.STREAM_SIZE ; i++)
    {
        printf("%d" , ls.STREAM[i]);
    }
    return 0;
}