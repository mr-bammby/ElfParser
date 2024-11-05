#include "../inc_pub/elfparser_strtable.h"
#include "../inc_priv/elfparser_memmanip_priv.h"

int ElfParser_StrTable_parse(elfparser_strtable_t *string_table, const void *map, size_t map_size)
{
    char *map_char = (char *)map;
    int64_t cnt = 0;
    size_t start = 0;
    int64_t ret = 0;

    if (string_table->table != NULL)
    {
        ElfParser_StrTable_free(string_table);
    }
    for (size_t i = 0; i < map_size; i++)
    {
        if (map_char[i] == '\0')
        {
            cnt++;
        }
    }
    string_table->table_size = (size_t)cnt;
    string_table->table = malloc(sizeof(char *) * string_table->table_size);
    if (string_table->table == NULL)
    {
        string_table->table_size = 0;
        return(-1);
    }
    cnt = 0;
    while (ret >= 0)
    {
        ret = ElfParser_strExtract(map, &(string_table->table[cnt]), start, map_size);
        start += ret;
        cnt++;
    }
    if (ret != -1)
    {
        if (ret != -4)
        {
            cnt--;
        } 
        for (;cnt >= 0; cnt--)
        {
            free(string_table->table[cnt]);
        }
        free(string_table->table);
        string_table->table_size = 0;
        string_table->table = NULL;
        return(-3);
    }
    return(0);
}

int ElfParser_StrTable_free(elfparser_strtable_t *string_table)
{
    if (string_table->table == NULL)
    {
        return(-1);
    }
    for( size_t cnt = 0; cnt < string_table->table_size; cnt++)
    {
        free(string_table->table[cnt]);
    }
    free(string_table->table);
    string_table->table = NULL;
    string_table->table_size = 0;
    return(0);
}
