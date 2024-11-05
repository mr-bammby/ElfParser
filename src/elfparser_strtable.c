#include "../inc_pub/elfparser_strtable.h"
#include "../inc_priv/elfparser_memmanip_priv.h"

int ElfParser_StrTable_parse(elfparser_strtable_t *string_table, const void *map, size_t map_size)
{
    char *map_char = (char *)map;
    int64_t cnt = 0;
    size_t start = 0;
    int64_t ret = 0;

    for (size_t i = 0; i < map_size; i++)
    {
        if (map_char[i] == '\0')
        {
            cnt++;
        }
    }
    *string_table = malloc(sizeof(char *) * cnt);
    if (*string_table == NULL)
    {
        return(-1);
    }
    cnt = 0;
    while (ret >= 0)
    {
        ret = ElfParser_strDup(map, &(*string_table[cnt]), start, map_size);
        start += ret;
    }
    if (ret != -1)
    {
       if (ret != -4)
       {
            cnt--;
       } 
       for (;cnt >= 0; cnt--)
       {
            free(*string_table[cnt]);
       }
       free(*string_table);
       return(-3);
    }
    return(0);


}