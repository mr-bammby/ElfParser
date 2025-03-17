/**
 * @file elfparser_secthead.h
 * @brief Public header for ELF section header parsing in libelfparser
 * @author Domen Banfi
 * @date 2025-03-16
 * @version 1.0
 *
 * This header provides the public interface for parsing ELF section headers within
 * the standalone libelfparser library. It includes definitions for section types,
 * flags, structures for section header data, and functions to manage section headers.
 */

#ifndef _IG_ELFPARSER_SECTHEAD_H_
#define _IG_ELFPARSER_SECTHEAD_H_

#include <inttypes.h>
#include <stdlib.h>
#include "../inc_pub/elfparser_common.h"
#include "../inc_pub/elfparser_header.h"

/* Section Type Constants (sh_type) - Corrected from macro list to enum-like defines */
#define ELFPARSER_SECTHEAD_TYPE_NULL       0x00u  /**< Null section */
#define ELFPARSER_SECTHEAD_TYPE_PROGBITS   0x01u /**< Program data */
#define ELFPARSER_SECTHEAD_TYPE_SYMTAB     0x02u /**< Symbol table */
#define ELFPARSER_SECTHEAD_TYPE_STRINGTAB  0x03u /**< String table */
#define ELFPARSER_SECTHEAD_TYPE_RELA       0x04u /**< Relocation entries with addends */
#define ELFPARSER_SECTHEAD_TYPE_HASH       0x05u /**< Symbol hash table */
#define ELFPARSER_SECTHEAD_TYPE_DYNAMIC    0x06u /**< Dynamic linking information */
#define ELFPARSER_SECTHEAD_TYPE_NOTE       0x07u /**< Notes */
#define ELFPARSER_SECTHEAD_TYPE_NOBITS     0x08u /**< No bits (uninitialized data) */
#define ELFPARSER_SECTHEAD_TYPE_REL        0x09u /**< Relocation entries without addends */
#define ELFPARSER_SECTHEAD_TYPE_SHLIB      0x0Au /**< Reserved for shared libraries */

/* Section Flag Constants (sh_flags) */
#define ELFPARSER_SECTHEAD_FLAG_WRITE           0x00000001u /**< Writable section */
#define ELFPARSER_SECTHEAD_FLAG_ALLOC           0x00000002u /**< Occupies memory during execution */
#define ELFPARSER_SECTHEAD_FLAG_EXECINST        0x00000004u /**< Executable instructions */
#define ELFPARSER_SECTHEAD_FLAG_MERGE           0x00000010u /**< Mergeable section */
#define ELFPARSER_SECTHEAD_FLAG_STRINGS         0x00000020u /**< Contains null-terminated strings */
#define ELFPARSER_SECTHEAD_FLAG_INFO_LINK       0x00000040u /**< sh_info contains section header index */
#define ELFPARSER_SECTHEAD_FLAG_LINK_ORDER      0x00000080u /**< Preserve link order */
#define ELFPARSER_SECTHEAD_FLAG_OS_NONCONFORM   0x00000100u /**< Non-standard OS-specific handling */
#define ELFPARSER_SECTHEAD_FLAG_GROUP           0x00000200u /**< Part of a section group */
#define ELFPARSER_SECTHEAD_FLAG_TLS             0x00000400u /**< Thread-local storage */
#define ELFPARSER_SECTHEAD_FLAG_MASK_OS         0x0ff00000u /**< OS-specific flags mask */
#define ELFPARSER_SECTHEAD_FLAG_MASK_PROC       0xf0000000u /**< Processor-specific flags mask */

/**
 * @brief Structure representing a single ELF section header entry
 */
typedef struct elfparser_secthead_entry_s
{
    char*     sh_name;       /**< Section name (dynamically allocated string) */
    uint32_t  sh_name_idx;   /**< Index of name in string table (sh_name) */
    uint32_t  sh_type;       /**< Section type (e.g., ELFPARSER_SECTHEAD_TYPE_*) */
    uint64_t  sh_flags;      /**< Section flags (e.g., ELFPARSER_SECTHEAD_FLAG_*) */
    uint64_t  sh_addr;       /**< Virtual address of section in memory */
    uint64_t  sh_offset;     /**< Offset of section in file */
    uint64_t  sh_size;       /**< Size of section in bytes */
    uint32_t  sh_link;       /**< Index of linked section (e.g., symbol table) */
    uint32_t  sh_info;       /**< Additional section info (context-dependent) */
    uint64_t  sh_addralign;  /**< Address alignment constraint */
    uint64_t  sh_entsize;    /**< Size of entries (if section has a table) */
} elfparser_secthead_entry_t;

/**
 * @brief Structure representing the ELF section header table
 */
typedef struct elfparser_secthead_s
{
    elfparser_secthead_entry_t* table;           /**< Array of section header entries */
    elfparser_header_class_e    elf_class;       /**< ELF class (32-bit or 64-bit) */
    elfparser_header_data_e     elf_data;        /**< Data encoding (endianness) */
    uint16_t                    table_len;       /**< Number of entries in table */
    uint16_t                    entry_size;      /**< Size of each entry in bytes */
    uint16_t                    string_table_idx; /**< Index of string table section */
    uint32_t                    max_idx;         /**< Maximum string table index encountered */
} elfparser_secthead_t;

/**
 * @brief Sets up the section header structure using ELF header data
 * @param[out] sect_head Pointer to the section header structure to initialize
 * @param[in] header Pointer to the ELF header containing section metadata
 * @return int ELFPARSER_SUCCESS on success, or an ElfParser_Error code on failure
 */
int ElfParser_SectHead_structSetup(elfparser_secthead_t *sect_head, const elfparser_header_t *header);

/**
 * @brief Parses the section header table from a memory map
 * @param[out] sect_head Pointer to the section header structure to populate
 * @param[in] map Pointer to the memory-mapped ELF file
 * @param[in] map_size Size of the memory map in bytes
 * @return int ELFPARSER_SUCCESS on success, or an ElfParser_Error code on failure
 */
int ElfParser_SectHead_parse(elfparser_secthead_t *sect_head, const void *map, size_t map_size);

/**
 * @brief Resolves section names from the string table
 * @param[in] sect_head Pointer to the section header structure
 * @param[in] map Pointer to the memory-mapped ELF file
 * @param[in] map_size Size of the memory map in bytes
 * @return int ELFPARSER_SUCCESS on success, or an ElfParser_Error code on failure
 */
int ElfParser_SectHead_nameResolve(const elfparser_secthead_t *sect_head, const void *map, size_t map_size);

/**
 * @brief Frees the section header structure and its allocated resources
 * @param[in,out] sect_head Pointer to the section header structure to free
 * @return int ELFPARSER_SUCCESS on success, or an ElfParser_Error code on failure
 */
int ElfParser_SectHead_free(elfparser_secthead_t *sect_head);

/**
 * @brief Finds a section header by name
 * @param[in] sect_head Pointer to the section header structure
 * @param[in] name Name of the section to find
 * @param[in] start_idx Starting index for the search
 * @return int32_t Index of the found section, ELFPARSER_SECTHEAD_NOT_FOUND if not found,
 *                 or an ElfParser_Error code on failure
 */
int32_t ElfParser_SectHead_byNameFind(const elfparser_secthead_t *sect_head, const char *name, size_t start_idx);

#endif /* _IG_ELFPARSER_SECTHEAD_H_ */