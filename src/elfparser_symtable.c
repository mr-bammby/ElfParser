#include "../inc_priv/elfparser_symtable_priv.h"
#include "../inc_pub/elfparser_secthead.h"
#include "../inc_priv/elfparser_memmanip_priv.h"
#include "../inc_pub/elfparser_symtable.h"
#include <stdlib.h>
#include <stdio.h>

int ElfParser_SymTable_structSetup(elfparser_symtable_t *symbol_table, const elfparser_secthead_t *sect_head, uint16_t symbol_table_sect_idx, const elfparser_header_t *header )
{
    int ret_val = 0;
    int32_t temp;

    symbol_table->elf_class = header->elf_ident.elf_class;
    symbol_table->elf_data = header->elf_ident.elf_data;
    symbol_table->entry_size = sect_head->table[symbol_table_sect_idx].sh_entsize;
    symbol_table->table_len = sect_head->table[symbol_table_sect_idx].sh_size/symbol_table->entry_size;
    temp = ElfParser_SectHead_byNameFind(sect_head, SYMTABLE_STRING_SECT_NAME, 0);
    if (temp < 0)
    {
        return(-2);
    }
    symbol_table->string_table_idx = (uint16_t)temp;
    symbol_table->max_idx = 0;
    symbol_table->table = malloc(symbol_table->table_len * sizeof(elfparser_symtable_entry_t ));
    if (symbol_table->table == NULL)
    {
        ret_val = -3;
    }
    return(ret_val);
}

int ElfParser_SymTable_parse(elfparser_symtable_t *symbol_table, const void *map, size_t map_size)
{
    int ret_val = 0;
    uint64_t* mem_off;
    uint64_t* mem_size;
    uint64_t mem_off_32bit[] = {    SYMTABLE_ENTRY_NAMEIDX_OFF,
                                    SYMTABLE_ENTRY_INFO_OFF,
                                    SYMTABLE_ENTRY_OTHER_OFF,
                                    SYMTABLE_ENTRY_SECTIDX_OFF,
                                    SYMTABLE_ENTRY_VALUE_OFF,
                                    SYMTABLE_ENTRY_SIZE_OFF_32BIT
                                    };

    uint64_t mem_off_64bit[] = {    SYMTABLE_ENTRY_NAMEIDX_OFF,
                                    SYMTABLE_ENTRY_INFO_OFF,
                                    SYMTABLE_ENTRY_OTHER_OFF,
                                    SYMTABLE_ENTRY_SECTIDX_OFF,
                                    SYMTABLE_ENTRY_VALUE_OFF,
                                    SYMTABLE_ENTRY_SIZE_OFF_64BIT
                                    };

    uint64_t mem_size_32bit[] = {    SYMTABLE_ENTRY_NAMEIDX_SIZE,
                                    SYMTABLE_ENTRY_INFO_SIZE,
                                    SYMTABLE_ENTRY_OTHER_SIZE,
                                    SYMTABLE_ENTRY_SECTIDX_SIZE,
                                    SYMTABLE_ENTRY_VALUE_SIZE_32BIT,
                                    SYMTABLE_ENTRY_SIZE_SIZE_32BIT
                                    };

    uint64_t mem_size_64bit[] = {    SYMTABLE_ENTRY_NAMEIDX_SIZE,
                                    SYMTABLE_ENTRY_INFO_SIZE,
                                    SYMTABLE_ENTRY_OTHER_SIZE,
                                    SYMTABLE_ENTRY_SECTIDX_SIZE,
                                    SYMTABLE_ENTRY_VALUE_SIZE_64BIT,
                                    SYMTABLE_ENTRY_SIZE_SIZE_64BIT
                                    };


    void *ret_dest[SYMTABLE_ENTRY_LEN];
    

    if ((symbol_table == NULL) || (map == NULL))
    {
        return (-2);
    }
    if ((map_size < (symbol_table->entry_size * symbol_table->table_len)) || ((symbol_table->entry_size * symbol_table->table_len) == 0))
    {
        return (-1);
    }

    if (symbol_table->elf_class == ELFPARSER_HEADER_CLASS_32_BIT)
    {
        mem_off = mem_off_32bit;
        mem_size = mem_size_32bit;
    }
    else if (symbol_table->elf_class == ELFPARSER_HEADER_CLASS_64_BIT)
    {
        mem_off = mem_off_64bit;
        mem_size = mem_size_64bit;
    }
    else
    {
        ret_val = -4;
    }
    if(ret_val == 0)
    {
        for (uint32_t i = 0; i < symbol_table->table_len; i++)
        {
            symbol_table->table[i].sym_name = NULL;
            symbol_table->table[i].sym_name_idx = 0u;
            symbol_table->table[i].sym_bind = 0u;
            symbol_table->table[i].sym_type = 0u;
            symbol_table->table[i].sym_visibility = 0u;
            symbol_table->table[i].sym_sect_idx = 0u;
            symbol_table->table[i].sym_value = 0u;
            symbol_table->table[i].sym_size = 0u;

            ret_dest[0] = (void *)&(symbol_table->table[i].sym_name_idx);
            ret_dest[1] = (void *)&(symbol_table->table[i].sym_type);
            ret_dest[2] = (void *)&(symbol_table->table[i].sym_visibility);
            ret_dest[3] = (void *)&(symbol_table->table[i].sym_sect_idx);
            ret_dest[4] = (void *)&(symbol_table->table[i].sym_value);
            ret_dest[5] = (void *)&(symbol_table->table[i].sym_size);
            for (uint8_t j = 0; j < SYMTABLE_ENTRY_LEN; j++)
            {
                if (symbol_table->elf_data == ELFPARSER_HEADER_DATA_BIG_ENDIANNESS)
                {
                    ElfParser_memRevCpy(ret_dest[j], map + (i * symbol_table->entry_size) + mem_off[j], mem_size[j]);
                }
                else if (symbol_table->elf_data  == ELFPARSER_HEADER_DATA_LITTLE_ENDIANNESS)
                {
                    ElfParser_memCpy(ret_dest[j], map + (i * symbol_table->entry_size) + mem_off[j], mem_size[j]);

                }
                else
                {
                    ret_val = -4;
                }
            }
            if (symbol_table->table[i].sym_name_idx > symbol_table->max_idx)
            {
                symbol_table->max_idx = symbol_table->table[i].sym_name_idx;
            }
            symbol_table->table[i].sym_bind = symbol_table->table[i].sym_type >> 4u;
            symbol_table->table[i].sym_type &= 0x0f;

        }
    }
    return (ret_val);
}

int ElfParser_SymTable_nameResolve(const elfparser_symtable_t *symbol_table, const void *map, size_t map_size)
{
    int64_t temp;
    int ret_val = 0;
    const char *char_map = map;
    
    if ((map_size <= symbol_table->max_idx) && (char_map[map_size - 1] != '\0'))
    {
        ret_val = -1;
    }
    for (size_t cnt = 0; cnt < symbol_table->table_len; cnt++)
    {
        temp = ElfParser_strDup(&char_map[symbol_table->table[cnt].sym_name_idx], &(symbol_table->table[cnt].sym_name));
        if (temp < 0)
        {
            ret_val = -2;
        }
    }
    return(ret_val);
}

int ElfParser_SymTable_free(elfparser_symtable_t *symbol_table)
{
    if ((symbol_table == NULL) || (symbol_table->table == NULL))
    {
        return(-1);
    }
    for (size_t cnt = 0; cnt < symbol_table->table_len; cnt++)
    {
        if (symbol_table->table[cnt].sym_name != NULL)
        {
            free(symbol_table->table[cnt].sym_name);
        }
    }
    free(symbol_table->table);
    symbol_table->table = NULL;
    return(0);
}

int32_t ElfParser_SymTable_byNameFind(elfparser_symtable_t const *symbol_table, const char *name, size_t start_idx)
{
    if((symbol_table == NULL) || (name == NULL))
    {
        return(-1);
    }
    if((symbol_table->table == NULL) || (symbol_table->table_len <= start_idx))
    {
        return(-1);
    }
    for (size_t cnt = start_idx; cnt < symbol_table->table_len; cnt++)
    {
        if (ElfParser_strCmp(symbol_table->table[cnt].sym_name, name) == 0)
        {
            return(cnt);
        }
    }
    return(-1);
}