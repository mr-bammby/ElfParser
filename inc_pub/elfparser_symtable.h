#ifndef _IG_ELFPARSER_SYMTABLE_H_
#define _IG_ELFPARSER_SYMTABLE_H_

#include <inttypes.h>
#include <stdlib.h>
#include "../inc_pub/elfparser_secthead.h"
#include "../inc_pub/elfparser_header.h"

#define ELFPARSER_SYMTABLE_BIND_NUM         0x03

#define ELFPARSER_SYMTABLE_BIND_LOCAL       0x00
#define ELFPARSER_SYMTABLE_BIND_GLOBAL      0x01
#define ELFPARSER_SYMTABLE_BIND_WEAK        0x02

#define ELFPARSER_SYMTABLE_BIND_GNU_UNIQUE  0x10
#define ELFPARSER_SYMTABLE_BIND_OSSPEC_LO   0x10
#define ELFPARSER_SYMTABLE_BIND_OSSPEC_HI   0x12
#define ELFPARSER_SYMTABLE_BIND_PROCSPEC_LO 0x13
#define ELFPARSER_SYMTABLE_BIND_PROCSPEC_HI 0x15

#define ELFPARSER_SYMTABLE_TYPE_NUM         0x07

#define ELFPARSER_SYMTABLE_TYPE_NOTYPE      0x00
#define ELFPARSER_SYMTABLE_TYPE_OBJECT      0x01
#define ELFPARSER_SYMTABLE_TYPE_FUNC        0x02
#define ELFPARSER_SYMTABLE_TYPE_SECT        0x03
#define ELFPARSER_SYMTABLE_TYPE_FILE        0x04
#define ELFPARSER_SYMTABLE_TYPE_COMMON      0x05
#define ELFPARSER_SYMTABLE_TYPE_TLS         0x06

#define ELFPARSER_SYMTABLE_TYPE_GNU_IFUNC   0x10
#define ELFPARSER_SYMTABLE_TYPE_OSSPEC_LO   0x10
#define ELFPARSER_SYMTABLE_TYPE_OSSPEC_HI   0x12
#define ELFPARSER_SYMTABLE_TYPE_PROCSPEC_LO 0x13
#define ELFPARSER_SYMTABLE_TYPE_PROCSPEC_HI 0x15

#define ELFPARSER_SYMTABLE_VISIBILITY_DEFAULT   0x00
#define ELFPARSER_SYMTABLE_VISIBILITY_INTERNAL  0x01
#define ELFPARSER_SYMTABLE_VISIBILITY_HIDDEN    0x02
#define ELFPARSER_SYMTABLE_VISIBILITY_PROTECTED 0x03

typedef struct elfparser_symtable_entry_s
{
    char* sym_name;
    uint32_t sym_name_idx; /* index of name in string table */
    uint8_t sym_bind;
    uint8_t sym_type;
    uint8_t sym_visibility;
    uint16_t sym_sect_idx;
    uint64_t sym_value;
    uint64_t sym_size;
} elfparser_symtable_entry_t;

typedef struct elfparser_symtable_s
{
    elfparser_symtable_entry_t* table;
    elfparser_header_class_e elf_class;
    elfparser_header_data_e elf_data;
    uint16_t table_len;
    uint16_t entry_size;
    uint16_t string_table_idx;
    uint32_t max_idx;
} elfparser_symtable_t;

int ElfParser_SymTable_structSetup(elfparser_symtable_t *symbol_table, const elfparser_secthead_t *sect_head, uint16_t symbol_table_sect_idx, const elfparser_header_t *header );
int ElfParser_SymTable_parse(elfparser_symtable_t *symbol_table, const void *map, size_t map_size);
int ElfParser_SymTable_nameResolve(const elfparser_symtable_t *symbol_table, const void *map, size_t map_size);
int ElfParser_SymTable_free(elfparser_symtable_t *symbol_table);
int32_t ElfParser_SymTable_byNameFind(elfparser_symtable_t const *symbol_table, const char *name, size_t start_idx);


#endif /* _IG_ELFPARSER_SYMTABLE_H_ */