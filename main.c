#include<stdio.h>
#include"LZW.h"

int main()
{
    struct LZW_table tab = Construct_Table(20 , 10);
    char ck[1] = {"A"};
    for(int i = 0; i < 10 ; i++)
    {
        PushTo_table(&tab , ck , 1);
        ck[0] += 1;
    }
    for(int i = 0; i < 10 ; i++)
    {
        printf("%s\n" , (char *)tab.TABLE + (tab.STRING_SIZE * i));
    }
    return 0;
}