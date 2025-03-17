/**
 * @file elfparser_symtable.h
 * @brief Public header for ELF symbol table parsing in libelfparser
 * @author Domen Banfi
 * @date 2025-03-16
 * @version 1.0
 *
 * This header provides the public interface for parsing ELF symbol tables within
 * the standalone libelfparser library. It includes definitions for symbol binding,
 * type, and visibility constants, structures for symbol table data, and functions
 * to manage symbol tables.
 */

#ifndef _IG_ELFPARSER_SYMTABLE_H_
#define _IG_ELFPARSER_SYMTABLE_H_

#include <inttypes.h>
#include <stdlib.h>
#include "../inc_pub/elfparser_common.h"
#include "../inc_pub/elfparser_secthead.h"
#include "../inc_pub/elfparser_header.h"

/* Symbol Binding Constants (st_info binding portion) */
#define ELFPARSER_SYMTABLE_BIND_NUM         0x03 /**< Number of standard binding types */
#define ELFPARSER_SYMTABLE_BIND_LOCAL       0x00 /**< Local symbol (STB_LOCAL) */
#define ELFPARSER_SYMTABLE_BIND_GLOBAL      0x01 /**< Global symbol (STB_GLOBAL) */
#define ELFPARSER_SYMTABLE_BIND_WEAK        0x02 /**< Weak symbol (STB_WEAK) */
#define ELFPARSER_SYMTABLE_BIND_GNU_UNIQUE  0x10 /**< GNU unique symbol (STB_GNU_UNIQUE) */
#define ELFPARSER_SYMTABLE_BIND_OSSPEC_LO   0x10 /**< Low end of OS-specific binding range */
#define ELFPARSER_SYMTABLE_BIND_OSSPEC_HI   0x12 /**< High end of OS-specific binding range */
#define ELFPARSER_SYMTABLE_BIND_PROCSPEC_LO 0x13 /**< Low end of processor-specific binding range */
#define ELFPARSER_SYMTABLE_BIND_PROCSPEC_HI 0x15 /**< High end of processor-specific binding range */

/* Symbol Type Constants (st_info type portion) */
#define ELFPARSER_SYMTABLE_TYPE_NUM         0x07 /**< Number of standard type values */
#define ELFPARSER_SYMTABLE_TYPE_NOTYPE      0x00 /**< Unspecified type (STT_NOTYPE) */
#define ELFPARSER_SYMTABLE_TYPE_OBJECT      0x01 /**< Data object (STT_OBJECT) */
#define ELFPARSER_SYMTABLE_TYPE_FUNC        0x02 /**< Function (STT_FUNC) */
#define ELFPARSER_SYMTABLE_TYPE_SECT        0x03 /**< Section (STT_SECTION) */
#define ELFPARSER_SYMTABLE_TYPE_FILE        0x04 /**< File name (STT_FILE) */
#define ELFPARSER_SYMTABLE_TYPE_COMMON      0x05 /**< Common data object (STT_COMMON) */
#define ELFPARSER_SYMTABLE_TYPE_TLS         0x06 /**< Thread-local storage (STT_TLS) */
#define ELFPARSER_SYMTABLE_TYPE_GNU_IFUNC   0x10 /**< GNU indirect function (STT_GNU_IFUNC) */
#define ELFPARSER_SYMTABLE_TYPE_OSSPEC_LO   0x10 /**< Low end of OS-specific type range */
#define ELFPARSER_SYMTABLE_TYPE_OSSPEC_HI   0x12 /**< High end of OS-specific type range */
#define ELFPARSER_SYMTABLE_TYPE_PROCSPEC_LO 0x13 /**< Low end of processor-specific type range */
#define ELFPARSER_SYMTABLE_TYPE_PROCSPEC_HI 0x15 /**< High end of processor-specific type range */

/* Symbol Visibility Constants (st_other) */
#define ELFPARSER_SYMTABLE_VISIBILITY_DEFAULT   0x00 /**< Default visibility (STV_DEFAULT) */
#define ELFPARSER_SYMTABLE_VISIBILITY_INTERNAL  0x01 /**< Internal visibility (STV_INTERNAL) */
#define ELFPARSER_SYMTABLE_VISIBILITY_HIDDEN    0x02 /**< Hidden visibility (STV_HIDDEN) */
#define ELFPARSER_SYMTABLE_VISIBILITY_PROTECTED 0x03 /**< Protected visibility (STV_PROTECTED) */

/* Return Value Constants */
#define ELFPARSER_SYMTABLE_NOT_FOUND -1 /**< Return value indicating symbol not found in ElfParser_SymTable_byNameFind */

/**
 * @brief Structure representing a single ELF symbol table entry
 */
typedef struct elfparser_symtable_entry_s
{
    char*    sym_name;       /**< Symbol name (dynamically allocated string) */
    uint32_t sym_name_idx;   /**< Index of name in string table (st_name) */
    uint8_t  sym_bind;       /**< Symbol binding (e.g., ELFPARSER_SYMTABLE_BIND_*) */
    uint8_t  sym_type;       /**< Symbol type (e.g., ELFPARSER_SYMTABLE_TYPE_*) */
    uint8_t  sym_visibility; /**< Symbol visibility (e.g., ELFPARSER_SYMTABLE_VISIBILITY_*) */
    uint16_t sym_sect_idx;   /**< Index of associated section (st_shndx) */
    uint64_t sym_value;      /**< Symbol value (st_value, address or offset) */
    uint64_t sym_size;       /**< Symbol size in bytes (st_size) */
} elfparser_symtable_entry_t;

/**
 * @brief Structure representing the ELF symbol table
 */
typedef struct elfparser_symtable_s
{
    elfparser_symtable_entry_t* table;           /**< Array of symbol table entries */
    elfparser_header_class_e    elf_class;       /**< ELF class (32-bit or 64-bit) */
    elfparser_header_data_e     elf_data;        /**< Data encoding (endianness) */
    uint16_t                    table_len;       /**< Number of entries in table */
    uint16_t                    entry_size;      /**< Size of each entry in bytes */
    uint16_t                    string_table_idx; /**< Index of string table section */
    uint32_t                    max_idx;         /**< Maximum string table index encountered */
} elfparser_symtable_t;

/**
 * @brief Sets up the symbol table structure using section and header data
 * @param[out] symbol_table Pointer to the symbol table structure to initialize
 * @param[in] sect_head Pointer to the section header structure
 * @param[in] symbol_table_sect_idx Index of the symbol table section in sect_head
 * @param[in] header Pointer to the ELF header containing class and endianness
 * @return int ELFPARSER_SUCCESS on success, or an ElfParser_Error code on failure
 */
int ElfParser_SymTable_structSetup(elfparser_symtable_t *symbol_table, const elfparser_secthead_t *sect_head, uint16_t symbol_table_sect_idx, const elfparser_header_t *header);

/**
 * @brief Parses the symbol table from a memory map
 * @param[out] symbol_table Pointer to the symbol table structure to populate
 * @param[in] map Pointer to the memory-mapped ELF file
 * @param[in] map_size Size of the memory map in bytes
 * @return int ELFPARSER_SUCCESS on success, or an ElfParser_Error code on failure
 */
int ElfParser_SymTable_parse(elfparser_symtable_t *symbol_table, const void *map, size_t map_size);

/**
 * @brief Resolves symbol names from the string table
 * @param[in] symbol_table Pointer to the symbol table structure
 * @param[in] map Pointer to the memory-mapped ELF file
 * @param[in] map_size Size of the memory map in bytes
 * @return int ELFPARSER_SUCCESS on success, or an ElfParser_Error code on failure
 */
int ElfParser_SymTable_nameResolve(const elfparser_symtable_t *symbol_table, const void *map, size_t map_size);

/**
 * @brief Frees the symbol table structure and its allocated resources
 * @param[in,out] symbol_table Pointer to the symbol table structure to free
 * @return int ELFPARSER_SUCCESS on success, or an ElfParser_Error code on failure
 */
int ElfParser_SymTable_free(elfparser_symtable_t *symbol_table);

/**
 * @brief Finds a symbol by name
 * @param[in] symbol_table Pointer to the symbol table structure
 * @param[in] name Name of the symbol to find
 * @param[in] start_idx Starting index for the search
 * @return int32_t Index of the found symbol, ELFPARSER_SYMTABLE_NOT_FOUND if not found,
 *                 or an ElfParser_Error code on failure
 */
int32_t ElfParser_SymTable_byNameFind(const elfparser_symtable_t *symbol_table, const char *name, size_t start_idx);

#endif /* _IG_ELFPARSER_SYMTABLE_H_ */