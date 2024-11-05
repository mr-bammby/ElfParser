#include "../inc_pub/elfparser_header.h"
#include "../inc_priv/elfparser_header_priv.h"
#include "../inc_priv/elfparser_memmanip_priv.h"
#include <string.h>
#include <stdio.h>

#define ELFPARSER_MAGIC_WORD "\177ELFF"

int  ElfParser_Header_identParse(elfparser_header_t *elf_header, const void *map, size_t size)
{
    char expected_magic_word[] = ELFPARSER_MAGIC_WORD;
    uint32_t mem_off[] = {  IDENT_MAGIC_OFF,
                            IDENT_CALSS_OFF,
                            IDENT_DATA_OFF,
                            IDENT_VERSION_OFF,
                            IDENT_OSABI_OFF,
                            IDENT_ABIVERSION_OFF};

    uint32_t mem_size[] = { IDENT_MAGIC_SIZE,
                            IDENT_CALSS_SIZE,
                            IDENT_DATA_SIZE,
                            IDENT_VERSION_SIZE,
                            IDENT_OSABI_SIZE,
                            IDENT_ABIVERSION_SIZE};

    void *ret_dest[] = {    (void *)&(elf_header->elf_ident.elf_magic_num),
                            (void *)&(elf_header->elf_ident.elf_class),
                            (void *)&(elf_header->elf_ident.elf_data),
                            (void *)&(elf_header->elf_ident.elf_version),
                            (void *)&(elf_header->elf_ident.elf_osabi),
                            (void *)&(elf_header->elf_ident.elf_abi_version)};
    if (size < 16)
    {
        return (-1);
    }
    if ((elf_header == NULL) || (map == NULL))
    {
        return (-2);
    }
    if(ElfParser_memCmp(map, (void *)&expected_magic_word, 4) != 0)
    {
        return(-3);
    }
    elf_header->elf_ident.elf_class = 0u;
    elf_header->elf_ident.elf_data = 0u;
    elf_header->elf_ident.elf_version = 0u;
    elf_header->elf_ident.elf_osabi = 0u;
    elf_header->elf_ident.elf_abi_version = 0u;
    for (uint8_t i = 0; i < IDENT_ENTRY_NUM; i++)
    {
        ElfParser_memCpy(ret_dest[i], map + mem_off[i], mem_size[i]);
    }
    return (0);
}

size_t  ElfParser_Header_sizeGet(elfparser_header_t *elf_header)
{
    if ((elf_header == NULL))
    {
        return (0);
    }
    if (elf_header->elf_ident.elf_class == ELFPARSER_HEADER_CLASS_32_BIT)
    {
        return (ELFPARSER_HEADER_SIZE_32BIT);
    }
    else if (elf_header->elf_ident.elf_class == ELFPARSER_HEADER_CLASS_64_BIT)
    {
        return (ELFPARSER_HEADER_SIZE_64BIT);
    }
    else
    {
        return (0);
    }
}


int  ElfParser_Header_parse(elfparser_header_t *elf_header, const void *map, size_t size)
{
    int ret_val = 0;
    uint32_t* mem_off;
    uint32_t* mem_size;
    uint32_t mem_off_32bit[] = {    HEADER_TYPE_OFF,
                                    HEADER_MACHINE_OFF,
                                    HEADER_VERSION_OFF,
                                    HEADER_ENTRY_OFF,
                                    HEADER_PROGTABLEOFF_OFF_32BIT,
                                    HEADER_SECTTABLEOFF_OFF_32BIT,
                                    HEADER_FLAGS_OFF_32BIT,
                                    HEADER_HEADERSIZE_OFF_32BIT,
                                    HEADER_PROGTENTSIZE_OFF_32BIT,
                                    HEADER_PROGTENTNUM_OFF_32BIT,
                                    HEADER_SECTTENTSIZE_OFF_32BIT,
                                    HEADER_SECTTENTNUM_OFF_32BIT,
                                    HEADER_SECTTENTNAMEIDX_OFF_32BIT};

    uint32_t mem_off_64bit[] = {    HEADER_TYPE_OFF,
                                    HEADER_MACHINE_OFF,
                                    HEADER_VERSION_OFF,
                                    HEADER_ENTRY_OFF,
                                    HEADER_PROGTABLEOFF_OFF_64BIT,
                                    HEADER_SECTTABLEOFF_OFF_64BIT,
                                    HEADER_FLAGS_OFF_64BIT,
                                    HEADER_HEADERSIZE_OFF_64BIT,
                                    HEADER_PROGTENTSIZE_OFF_64BIT,
                                    HEADER_PROGTENTNUM_OFF_64BIT,
                                    HEADER_SECTTENTSIZE_OFF_64BIT,
                                    HEADER_SECTTENTNUM_OFF_64BIT,
                                    HEADER_SECTTENTNAMEIDX_OFF_64BIT};

    uint32_t mem_size_32bit[] = {   HEADER_TYPE_SIZE,
                                    HEADER_MACHINE_SIZE,
                                    HEADER_VERSION_SIZE,
                                    HEADER_ENTRY_SIZE_32BIT,
                                    HEADER_PROGTABLEOFF_SIZE_32BIT,
                                    HEADER_SECTTABLEOFF_SIZE_32BIT,
                                    HEADER_FLAGS_SIZE,
                                    HEADER_HEADERSIZE_SIZE,
                                    HEADER_PROGTENTSIZE_SIZE,
                                    HEADER_PROGTENTNUM_SIZE,
                                    HEADER_SECTTENTSIZE_SIZE,
                                    HEADER_SECTTENTNUM_SIZE,
                                    HEADER_SECTTENTNAMEIDX_SIZE};

    uint32_t mem_size_64bit[] = {   HEADER_TYPE_SIZE,
                                    HEADER_MACHINE_SIZE,
                                    HEADER_VERSION_SIZE,
                                    HEADER_ENTRY_SIZE_64BIT,
                                    HEADER_PROGTABLEOFF_SIZE_64BIT,
                                    HEADER_SECTTABLEOFF_SIZE_64BIT,
                                    HEADER_FLAGS_SIZE,
                                    HEADER_HEADERSIZE_SIZE,
                                    HEADER_PROGTENTSIZE_SIZE,
                                    HEADER_PROGTENTNUM_SIZE,
                                    HEADER_SECTTENTSIZE_SIZE,
                                    HEADER_SECTTENTNUM_SIZE,
                                    HEADER_SECTTENTNAMEIDX_SIZE};

    void *ret_dest[] = {    (void *)&(elf_header->elf_type),
                            (void *)&(elf_header->elf_machine),
                            (void *)&(elf_header->elf_version),
                            (void *)&(elf_header->elf_entry),
                            (void *)&(elf_header->elf_program_header_off),
                            (void *)&(elf_header->elf_section_header_off),
                            (void *)&(elf_header->elf_flags),
                            (void *)&(elf_header->elf_header_size),
                            (void *)&(elf_header->elf_program_header_entry_size),
                            (void *)&(elf_header->elf_program_header_entry_num),
                            (void *)&(elf_header->elf_section_header_entry_size),
                            (void *)&(elf_header->elf_section_header_entry_num),
                            (void *)&(elf_header->elf_section_header_name_idx)};
    

    if ((elf_header == NULL) || (map == NULL))
    {
        return (-2);
    }
    if ((size < ElfParser_Header_sizeGet(elf_header)) || (ElfParser_Header_sizeGet(elf_header) == 0))
    {
        return (-1);
    }

    if (elf_header->elf_ident.elf_class == ELFPARSER_HEADER_CLASS_32_BIT)
    {
        mem_off = mem_off_32bit;
        mem_size = mem_size_32bit;
    }
    else if (elf_header->elf_ident.elf_class == ELFPARSER_HEADER_CLASS_64_BIT)
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
        elf_header->elf_type = 0u;
        elf_header->elf_machine = 0u;
        elf_header->elf_version = 0u;
        elf_header->elf_entry = 0u;
        elf_header->elf_program_header_off = 0u;
        elf_header->elf_section_header_off = 0u;
        elf_header->elf_flags = 0u;
        elf_header->elf_header_size = 0u;
        elf_header->elf_program_header_entry_size = 0u;
        elf_header->elf_program_header_entry_num = 0u;
        elf_header->elf_section_header_entry_size = 0u;
        elf_header->elf_section_header_entry_num = 0u;
        elf_header->elf_section_header_name_idx = 0u;
        if (elf_header->elf_ident.elf_data == ELFPARSER_HEADER_DATA_BIG_ENDIANNESS)
        {
            for (uint8_t i = 0; i < HEADER_ENTRY_NUM; i++)
            {
                ElfParser_memRevCpy(ret_dest[i], map + mem_off[i], mem_size[i]);
            }
        }
        else if (elf_header->elf_ident.elf_data == ELFPARSER_HEADER_DATA_LITTLE_ENDIANNESS)
        {
            for (uint8_t i = 0; i < HEADER_ENTRY_NUM; i++)
            {
                ElfParser_memCpy(ret_dest[i], map + mem_off[i], mem_size[i]);
            }
        }
        else
        {
            ret_val = -4;
        }
    }

    return (ret_val);
}
