#include "../inc_priv/elfparser_secthead_priv.h"
#include "../inc_pub/elfparser_secthead.h"
#include "../inc_priv/elfparser_memmanip_priv.h"
#include "../inc_pub/elfparser_header.h"
#include <stdlib.h>
#include <stdio.h>

int ElfParser_SectHead_structSetup(elfparser_secthead_t *sect_head,  const elfparser_header_t *header)
{
    int ret_val = 0;

    sect_head->elf_class = header->elf_ident.elf_class;
    sect_head->elf_data = header->elf_ident.elf_data;
    sect_head->entry_size = header->elf_section_header_entry_size;
    sect_head->table_len = header->elf_section_header_entry_num;
    sect_head->string_table_idx = header->elf_section_header_name_idx;
    sect_head->max_idx = 0;
    sect_head->table = malloc(sect_head->table_len * sizeof(elfparser_secthead_entry_t));
    if (sect_head->table == NULL)
    {
        ret_val = -3;
    }
    return(ret_val);
}

int  ElfParser_SectHead_parse(elfparser_secthead_t *sect_head, const void *map, size_t map_size)
{
    int ret_val = 0;
    uint64_t* mem_off;
    uint64_t* mem_size;
    uint64_t mem_off_32bit[] = {    SECTHEADER_ENTRY_NAMEIDX_OFF,
                                    SECTHEADER_ENTRY_TYPE_OFF,
                                    SECTHEADER_ENTRY_FLAGS_OFF,
                                    SECTHEADER_ENTRY_SECTADDR_OFF_32BIT,
                                    SECTHEADER_ENTRY_SECTOFF_OFF_32BIT,
                                    SECTHEADER_ENTRY_SECTSIZE_OFF_32BIT,
                                    SECTHEADER_ENTRY_LINK_OFF_32BIT,
                                    SECTHEADER_ENTRY_INFO_OFF_32BIT,
                                    SECTHEADER_ENTRY_ADDRALIGN_OFF_32BIT,
                                    SECTHEADER_ENTRY_ENTRYSIZE_OFF_32BIT
                                    };

    uint64_t mem_off_64bit[] = {    SECTHEADER_ENTRY_NAMEIDX_OFF,
                                    SECTHEADER_ENTRY_TYPE_OFF,
                                    SECTHEADER_ENTRY_FLAGS_OFF,
                                    SECTHEADER_ENTRY_SECTADDR_OFF_64BIT,
                                    SECTHEADER_ENTRY_SECTOFF_OFF_64BIT,
                                    SECTHEADER_ENTRY_SECTSIZE_OFF_64BIT,
                                    SECTHEADER_ENTRY_LINK_OFF_64BIT,
                                    SECTHEADER_ENTRY_INFO_OFF_64BIT,
                                    SECTHEADER_ENTRY_ADDRALIGN_OFF_64BIT,
                                    SECTHEADER_ENTRY_ENTRYSIZE_OFF_64BIT
                                    };

    uint64_t mem_size_32bit[] = {   SECTHEADER_ENTRY_NAMEIDX_SIZE,
                                    SECTHEADER_ENTRY_TYPE_SIZE,
                                    SECTHEADER_ENTRY_FLAGS_SIZE_32BIT,
                                    SECTHEADER_ENTRY_SECTADDR_SIZE_32BIT,
                                    SECTHEADER_ENTRY_SECTOFF_SIZE_32BIT,
                                    SECTHEADER_ENTRY_SECTSIZE_SIZE_32BIT,
                                    SECTHEADER_ENTRY_LINK_SIZE,
                                    SECTHEADER_ENTRY_INFO_SIZE,
                                    SECTHEADER_ENTRY_ADDRALIGN_SIZE_32BIT,
                                    SECTHEADER_ENTRY_ENTRYSIZE_SIZE_32BIT
                                    };

    uint64_t mem_size_64bit[] = {   SECTHEADER_ENTRY_NAMEIDX_SIZE,
                                    SECTHEADER_ENTRY_TYPE_SIZE,
                                    SECTHEADER_ENTRY_FLAGS_SIZE_64BIT,
                                    SECTHEADER_ENTRY_SECTADDR_SIZE_64BIT,
                                    SECTHEADER_ENTRY_SECTOFF_SIZE_64BIT,
                                    SECTHEADER_ENTRY_SECTSIZE_SIZE_64BIT,
                                    SECTHEADER_ENTRY_LINK_SIZE,
                                    SECTHEADER_ENTRY_INFO_SIZE,
                                    SECTHEADER_ENTRY_ADDRALIGN_SIZE_64BIT,
                                    SECTHEADER_ENTRY_ENTRYSIZE_SIZE_64BIT
                                    };


    void *ret_dest[SECTHEADER_ENTRY_LEN];
    

    if ((sect_head == NULL) || (map == NULL))
    {
        return (-2);
    }
    if ((map_size < (sect_head->entry_size * sect_head->table_len)) || ((sect_head->entry_size * sect_head->table_len) == 0))
    {
        return (-1);
    }

    if (sect_head->elf_class == ELFPARSER_HEADER_CLASS_32_BIT)
    {
        mem_off = mem_off_32bit;
        mem_size = mem_size_32bit;
    }
    else if (sect_head->elf_class == ELFPARSER_HEADER_CLASS_64_BIT)
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
        for (uint32_t i = 0; i < sect_head->table_len; i++)
        {
            sect_head->table[i].sh_name = NULL;
            sect_head->table[i].sh_name_idx = 0u;
            sect_head->table[i].sh_type = 0u;
            sect_head->table[i].sh_flags = 0u;
            sect_head->table[i].sh_addr = 0u;
            sect_head->table[i].sh_offset = 0u;
            sect_head->table[i].sh_size = 0u;
            sect_head->table[i].sh_link = 0u;
            sect_head->table[i].sh_info = 0u;
            sect_head->table[i].sh_addralign = 0u;
            sect_head->table[i].sh_entsize = 0u;

            ret_dest[0] = (void *)&(sect_head->table[i].sh_name_idx);
            ret_dest[1] = (void *)&(sect_head->table[i].sh_type);
            ret_dest[2] = (void *)&(sect_head->table[i].sh_flags);
            ret_dest[3] = (void *)&(sect_head->table[i].sh_addr);
            ret_dest[4] = (void *)&(sect_head->table[i].sh_offset);
            ret_dest[5] = (void *)&(sect_head->table[i].sh_size);
            ret_dest[6] = (void *)&(sect_head->table[i].sh_link);
            ret_dest[7] = (void *)&(sect_head->table[i].sh_info);
            ret_dest[8] = (void *)&(sect_head->table[i].sh_addralign);
            ret_dest[9] = (void *)&(sect_head->table[i].sh_entsize);
            for (uint8_t j = 0; j < SECTHEADER_ENTRY_LEN; j++)
            {
                if (sect_head->elf_data == ELFPARSER_HEADER_DATA_BIG_ENDIANNESS)
                {
                    ElfParser_memRevCpy(ret_dest[j], map + (i * sect_head->entry_size) + mem_off[j], mem_size[j]);
                }
                else if (sect_head->elf_data  == ELFPARSER_HEADER_DATA_LITTLE_ENDIANNESS)
                {
                    ElfParser_memCpy(ret_dest[j], map + (i * sect_head->entry_size) + mem_off[j], mem_size[j]);

                }
                else
                {
                    ret_val = -4;
                }
            }
            if (sect_head->table[i].sh_name_idx > sect_head->max_idx)
            {
                sect_head->max_idx = sect_head->table[i].sh_name_idx;
            }
        }
    }

    return (ret_val);
}

int ElfParser_SectHead_nameResolve(const elfparser_secthead_t *sect_head, const void *map, size_t map_size)
{
    int64_t temp;
    int ret_val = 0;
    const char *char_map = map;
    
    if ((map_size < sect_head->max_idx) && (char_map[map_size - 1] != '\0'))
    {
        ret_val = -1;
    }
    for (size_t cnt = 0; cnt < sect_head->table_len; cnt++)
    {
        printf("%ld\n", cnt);
        temp = ElfParser_strDup(&char_map[sect_head->table[cnt].sh_name_idx], &(sect_head->table[cnt].sh_name));
        if (temp < 0)
        {
            ret_val = -2;
        }
    }
    return(ret_val);
}

int ElfParser_SectHead_free(elfparser_secthead_t *sect_head)
{
    if ((sect_head == NULL) || (sect_head->table == NULL))
    {
        return(-1);
    }
    for (size_t cnt = 0; cnt < sect_head->table_len; cnt++)
    {
        if (sect_head->table[cnt].sh_name != NULL)
        {
            free(sect_head->table[cnt].sh_name);
        }
    }
    free(sect_head->table);
    sect_head->table = NULL;
    return(0);
}

int32_t ElfParser_SectHead_byNameFind(elfparser_secthead_t const *sect_head, const char *name, size_t start_idx)
{
    if((sect_head == NULL) || (name == NULL))
    {
        return(-1);
    }
    if((sect_head->table == NULL) || (sect_head->table_len <= start_idx))
    {
        return(-2);
    }
    for (size_t cnt = start_idx; cnt < sect_head->table_len; cnt++)
    {
        if (ElfParser_strCmp(sect_head->table[cnt].sh_name, name) == 0)
        {
            return (cnt);
        }
    }
    return(-1);
}