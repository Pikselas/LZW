#pragma once
#include<stdlib.h>
struct LZW_table
{
    int STRING_SIZE;
    int TABLE_SIZE;
    void * TABLE;
};

struct LZW_table Construct_Table(int string_size , int table_size)
{
    struct LZW_table table;
    table.STRING_SIZE = string_size;
    table.TABLE_SIZE = table_size;
    table.TABLE = malloc(sizeof(char) * string_size * table_size);
    return table;
}
void Destruct_Table(struct LZW_table * table)
{
    table->STRING_SIZE = 0;
    table->TABLE_SIZE = 0;
    free(table->TABLE);
}