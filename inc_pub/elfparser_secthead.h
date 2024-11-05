#ifndef _IG_ELFPARSER_SECTHEAD_H_
#define _IG_ELFPARSER_SECTHEAD_H_

#include <inttypes.h>
#include <stdlib.h>
#include "../inc_pub/elfparser_header.h"

#define ELFPARSER_SECTHEAD_TYPE_NULL      = 0x0u,
#define ELFPARSER_SECTHEAD_TYPE_PROGBITS  = 0x01u,
#define ELFPARSER_SECTHEAD_TYPE_SYMTAB    = 0x02u,
#define ELFPARSER_SECTHEAD_TYPE_STRINGTAB = 0x03u,
#define ELFPARSER_SECTHEAD_TYPE_RELA      = 0x04u,
#define ELFPARSER_SECTHEAD_TYPE_HASH      = 0x05u,
#define ELFPARSER_SECTHEAD_TYPE_DYNAMIC   = 0x06u,
#define ELFPARSER_SECTHEAD_TYPE_NOTE      = 0x07u,
#define ELFPARSER_SECTHEAD_TYPE_NOBITS    = 0x08u,
#define ELFPARSER_SECTHEAD_TYPE_REL       = 0x09u,
#define ELFPARSER_SECTHEAD_TYPE_SHLIB     = 0x0Au


#define ELFPARSER_SECTHEAD_FLAG_WTITE         0x00000001u
#define ELFPARSER_SECTHEAD_FLAG_ALLOC         0x00000002u
#define ELFPARSER_SECTHEAD_FLAG_EXECINST      0x00000004u
#define ELFPARSER_SECTHEAD_FLAG_MERGE         0x00000010u
#define ELFPARSER_SECTHEAD_FLAG_STRINGS       0x00000020u
#define ELFPARSER_SECTHEAD_FLAG_INFO_LINK     0x00000040u
#define ELFPARSER_SECTHEAD_FLAG_LINK_ORDER    0x00000080u
#define ELFPARSER_SECTHEAD_FLAG_OS_NONCONFORM 0x00000100u
#define ELFPARSER_SECTHEAD_FLAG_GROUP         0x00000200u
#define ELFPARSER_SECTHEAD_FLAG_TLS           0x00000400u
#define ELFPARSER_SECTHEAD_FLAG_MASK_OS       0x0ff00000u
#define ELFPARSER_SECTHEAD_FLAG_MASK_PROC     0xf0000000u

typedef struct elfparser_secthead_entry_s
{
    char* sh_name;
    uint32_t sh_name_idx; /* index of name in string table */
    uint32_t sh_type;
    uint64_t sh_flags;
    uint64_t sh_addr;
    uint64_t sh_offset;
    uint64_t sh_size;
    uint32_t sh_link;
    uint32_t sh_info;
    uint64_t sh_addralign;
    uint64_t sh_entsize;
} elfparser_secthead_entry_t;

typedef struct elfparser_secthead_s
{
    elfparser_secthead_entry_t* table;
    elfparser_header_class_e elf_class;
    elfparser_header_data_e elf_data;
    uint16_t table_len;
    uint16_t entry_size;
    uint16_t string_table_idx;
    uint32_t max_idx;
} elfparser_secthead_t;

int ElfParser_SectHead_structSetup(elfparser_secthead_t *sect_head, const elfparser_header_t *header);
int ElfParser_SectHead_parse(elfparser_secthead_t *sect_head, const void *map, size_t map_size);
int ElfParser_SectHead_nameResolve(const elfparser_secthead_t *sect_head, const void *map, size_t map_size);
int ElfParser_SectHead_free(elfparser_secthead_t *sect_head);
int32_t ElfParser_SectHead_byNameFind(elfparser_secthead_t const *sect_head, const char *name, size_t start_idx);

#endif /* _IG_ELFPARSER_SECTHEAD_H_ */