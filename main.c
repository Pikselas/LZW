#include<stdio.h>
#include"LZW.h"

int main()
{
    char cl [20][20] = {"hello","poppelo"};
    char (* pt)[20] = cl;
    printf("%s" , pt[0]);
    return 0;
}