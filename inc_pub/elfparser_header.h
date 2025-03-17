/**
 * @file elfparser_header.h
 * @brief Public header for ELF header parsing in libelfparser
 * @author Domen Banfi
 * @date 2025-03-16
 * @version 1.0
 *
 * This header provides the public interface for parsing ELF file headers within
 * the standalone libelfparser library. It includes definitions for header sizes,
 * enumerations for ELF attributes, structures for header data, error codes, and
 * functions to parse and query ELF headers.
 */

#ifndef _IG_ELFPARSER_HEADER_H_
#define _IG_ELFPARSER_HEADER_H_

#include <inttypes.h>
#include <stdlib.h>
#include "../inc_pub/elfparser_common.h"

#define ELFPARSER_HEADER_SIZE_32BIT 52u /**< Size of a 32-bit ELF header in bytes */
#define ELFPARSER_HEADER_SIZE_64BIT 64u /**< Size of a 64-bit ELF header in bytes */

/**
 * @brief Enumeration of ELF class types (EI_CLASS)
 */
typedef enum
{
    ELFPARSER_HEADER_CLASS_32_BIT = 1, /**< 32-bit ELF format */
    ELFPARSER_HEADER_CLASS_64_BIT = 2  /**< 64-bit ELF format */
} elfparser_header_class_e;

/**
 * @brief Enumeration of ELF data encoding types (EI_DATA)
 */
typedef enum
{
    ELFPARSER_HEADER_DATA_LITTLE_ENDIANNESS = 1, /**< Little-endian encoding */
    ELFPARSER_HEADER_DATA_BIG_ENDIANNESS    = 2  /**< Big-endian encoding */
} elfparser_header_data_e;

/**
 * @brief Enumeration of ELF OS/ABI identifiers (EI_OSABI)
 */
typedef enum
{
    ELFPARSER_HEADER_OSABI_SYS_V          = 0x00, /**< UNIX System V ABI */
    ELFPARSER_HEADER_OSABI_HP_UX          = 0x01, /**< HP-UX ABI */
    ELFPARSER_HEADER_OSABI_NETBDS         = 0x02, /**< NetBSD ABI (typo: likely NETBSD) */
    ELFPARSER_HEADER_OSABI_LINUX          = 0x03, /**< Linux ABI */
    ELFPARSER_HEADER_OSABI_GNU_HURD       = 0x04, /**< GNU Hurd ABI */
    ELFPARSER_HEADER_OSABI_SOLARIS        = 0x06, /**< Solaris ABI */
    ELFPARSER_HEADER_OSABI_AIX            = 0x07, /**< AIX ABI */
    ELFPARSER_HEADER_OSABI_IRIX           = 0x08, /**< IRIX ABI */
    ELFPARSER_HEADER_OSABI_FREEBSD        = 0x09, /**< FreeBSD ABI */
    ELFPARSER_HEADER_OSABI_TRU64          = 0x0A, /**< Tru64 UNIX ABI */
    ELFPARSER_HEADER_OSABI_NOVELL_MODESTO = 0x0B, /**< Novell Modesto ABI */
    ELFPARSER_HEADER_OSABI_OPENBDI        = 0x0C, /**< OpenBSD ABI (typo: likely OPENBSD) */
    ELFPARSER_HEADER_OSABI_OPENVMS        = 0x0D, /**< OpenVMS ABI */
    ELFPARSER_HEADER_OSABI_NONSTOP_KERNEL = 0x0E, /**< NonStop Kernel ABI */
    ELFPARSER_HEADER_OSABI_AROS           = 0x0F, /**< AROS ABI */
    ELFPARSER_HEADER_OSABI_FENIXOS        = 0x10, /**< FenixOS ABI */
    ELFPARSER_HEADER_OSABI_NUXI_CLOUDABI  = 0x11, /**< Nuxi CloudABI */
    ELFPARSER_HEADER_OSABI_OPENVOS        = 0x12  /**< OpenVOS ABI */
} elfparser_header_osabi_e;

/**
 * @brief Enumeration of ELF file types (e_type)
 */
typedef enum
{
    ELFPARSER_HEADER_TYPE_NONE = 0x00, /**< No file type */
    ELFPARSER_HEADER_TYPE_REL  = 0x01, /**< Relocatable file */
    ELFPARSER_HEADER_TYPE_EXEC = 0x02, /**< Executable file */
    ELFPARSER_HEADER_TYPE_DYN  = 0x03, /**< Shared object file */
    ELFPARSER_HEADER_TYPE_CORE = 0x04  /**< Core file */
} elfparser_header_type_e;

/**
 * @brief Structure representing the ELF identification block (e_ident)
 */
typedef struct elfparser_header_ident_s
{
    uint8_t                     elf_magic_num[4]; /**< Magic number (EI_MAG0 to EI_MAG3) */
    elfparser_header_class_e    elf_class;        /**< ELF class (32-bit or 64-bit) */
    elfparser_header_data_e     elf_data;         /**< Data encoding (endianness) */
    uint8_t                     elf_version;      /**< File version (EI_VERSION) */
    elfparser_header_osabi_e    elf_osabi;        /**< OS/ABI identifier */
    uint8_t                     elf_abi_version;  /**< ABI version (EI_ABIVERSION) */
} elfparser_header_ident_t;

/**
 * @brief Structure representing the full ELF header
 */
typedef struct elfparser_header_s
{
    elfparser_header_ident_t    elf_ident;                      /**< Identification block */
    elfparser_header_type_e     elf_type;                       /**< Object file type */
    uint16_t                    elf_machine;                    /**< Target machine architecture */
    uint64_t                    elf_version;                    /**< Object file version (stored as 64-bit for alignment) */
    uint64_t                    elf_entry;                      /**< Entry point virtual address */
    uint64_t                    elf_program_header_off;         /**< Program header table offset */
    uint64_t                    elf_section_header_off;         /**< Section header table offset */
    uint32_t                    elf_flags;                      /**< Processor-specific flags */
    uint16_t                    elf_header_size;                /**< Size of this header */
    uint16_t                    elf_program_header_entry_size;  /**< Size of a program header entry */
    uint16_t                    elf_program_header_entry_num;   /**< Number of program header entries */
    uint16_t                    elf_section_header_entry_size;  /**< Size of a section header entry */
    uint16_t                    elf_section_header_entry_num;   /**< Number of section header entries */
    uint16_t                    elf_section_header_name_idx;    /**< Index of section name string table */
} elfparser_header_t;

/**
 * @brief Parses the ELF identification block from a memory map
 * @param[out] elfparser_header Pointer to the ELF header structure to populate
 * @param[in] map Pointer to the memory-mapped ELF file
 * @param[in] size Size of the memory map in bytes
 * @return int ELFPARSER_SUCCESS on success, or an ElfParser_Error code on failure
 */
int ElfParser_Header_identParse(elfparser_header_t *elfparser_header, const void *map, size_t size);

/**
 * @brief Parses the full ELF header from a memory map
 * @param[out] elfparser_header Pointer to the ELF header structure to populate
 * @param[in] map Pointer to the memory-mapped ELF file
 * @param[in] size Size of the memory map in bytes
 * @return int ELFPARSER_SUCCESS on success, or an ElfParser_Error code on failure
 */
int ElfParser_Header_parse(elfparser_header_t *elfparser_header, const void *map, size_t size);

/**
 * @brief Retrieves the size of the ELF header based on its class
 * @param[in] elfparser_header Pointer to the ELF header structure
 * @return size_t Size of the header in bytes, or 0 if elfparser_header is NULL or class is invalid
 */
size_t ElfParser_Header_sizeGet(const elfparser_header_t *elfparser_header);

#endif /* _IG_ELFPARSER_HEADER_H_ */
