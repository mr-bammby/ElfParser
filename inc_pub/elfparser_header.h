#ifndef _IG_ELFPARSER_HEADER_H_
#define _IG_ELFPARSER_HEADER_H_

#include <inttypes.h>
#include <stdlib.h>

#define ELFPARSER_HEADER_SIZE_32BIT 52u
#define ELFPARSER_HEADER_SIZE_64BIT 64u

typedef enum
{
    ELFPARSER_HEADER_CLASS_32_BIT = 1,
    ELFPARSER_HEADER_CLASS_64_BIT = 2
} elfparser_header_class_e;

typedef enum
{
    ELFPARSER_HEADER_DATA_LITTLE_ENDIANNESS  = 1,
    ELFPARSER_HEADER_DATA_BIG_ENDIANNESS     = 2
} elfparser_header_data_e;

typedef enum
{
    ELFPARSER_HEADER_OSABI_SYS_V             = 0x00,
    ELFPARSER_HEADER_OSABI_HP_UX             = 0x01,
    ELFPARSER_HEADER_OSABI_NETBDS            = 0x02,
    ELFPARSER_HEADER_OSABI_LINUX             = 0x03,
    ELFPARSER_HEADER_OSABI_GNU_HURD          = 0x04,
    ELFPARSER_HEADER_OSABI_SOLARIS           = 0x06,
    ELFPARSER_HEADER_OSABI_AIX               = 0x07,
    ELFPARSER_HEADER_OSABI_IRIX              = 0x08,
    ELFPARSER_HEADER_OSABI_FREEBSD           = 0x09,
    ELFPARSER_HEADER_OSABI_TRU64             = 0x0A,
    ELFPARSER_HEADER_OSABI_NOVELL_MODESTO    = 0x0B,
    ELFPARSER_HEADER_OSABI_OPENBDI           = 0x0C,
    ELFPARSER_HEADER_OSABI_OPENVMS           = 0x0D,
    ELFPARSER_HEADER_OSABI_NONSTOP_KERNEL    = 0x0E,
    ELFPARSER_HEADER_OSABI_AROS              = 0x0F,
    ELFPARSER_HEADER_OSABI_FENIXOS           = 0x10,
    ELFPARSER_HEADER_OSABI_NUXI_CLOUDABI     = 0x11,
    ELFPARSER_HEADER_OSABI_OPENVOS           = 0x12
}   elfparser_header_osabi_e;


typedef enum
{
    ELFPARSER_HEADER_TYPE_NONE   = 0x00,
    ELFPARSER_HEADER_TYPE_REL    = 0x01,
    ELFPARSER_HEADER_TYPE_EXEC   = 0x02,
    ELFPARSER_HEADER_TYPE_DYN    = 0x03,
    ELFPARSER_HEADER_TYPE_CORE   = 0x04
}   elfparser_header_type_e;


typedef struct elfparser_header_ident_s
{
    uint8_t                     elf_magic_num[4];
    elfparser_header_class_e    elf_class;
    elfparser_header_data_e     elf_data;
    uint8_t                     elf_version;
    elfparser_header_osabi_e    elf_osabi;
    uint8_t                     elf_abi_version;
} elfparser_header_ident_t;


typedef struct elfparser_header_s
{
    elfparser_header_ident_t    elf_ident;
    elfparser_header_type_e     elf_type;
    uint16_t                    elf_machine;
    uint64_t                    elf_version;
    uint64_t                    elf_entry;
    uint64_t                    elf_program_header_off;
    uint64_t                    elf_section_header_off;
    uint32_t                    elf_flags;
    uint16_t                    elf_header_size;
    uint16_t                    elf_program_header_entry_size;
    uint16_t                    elf_program_header_entry_num;
    uint16_t                    elf_section_header_entry_size;
    uint16_t                    elf_section_header_entry_num;
    uint16_t                    elf_section_header_name_idx;
} elfparser_header_t;


int  ElfParser_Header_identParse(elfparser_header_t *elfparser_header, const void *map, size_t size);
int  ElfParser_Header_parse(elfparser_header_t *elfparser_header, const void *map, size_t size);
size_t ElfParser_Header_sizeGet(elfparser_header_t *elfparser_header);

#endif /* _IG_ELFPARSER_HEADER_H_ */