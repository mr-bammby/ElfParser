/**
 * @file elfparser_symtable.c
 * @brief ELF symbol table parsing functions for libelfparser
 * @author Domen Banfi
 * @date 2025-03-16
 * @version 1.0
 *
 * This file implements functions to parse and manage ELF symbol tables within
 * the standalone libelfparser library. It supports setting up symbol table
 * structures, parsing symbol tables, resolving symbol names, and freeing
 * allocated resources, for both 32-bit and 64-bit ELF formats.
 */

#include "../inc_priv/elfparser_symtable_priv.h"
#include "../inc_pub/elfparser_secthead.h"
#include "../inc_priv/elfparser_memmanip_priv.h"
#include "../inc_pub/elfparser_symtable.h"
#include <stdlib.h>

/**
 * @brief Sets up the symbol table structure using section and header data
 * @param[out] symbol_table Pointer to the symbol table structure to initialize
 * @param[in] sect_head Pointer to the section header structure
 * @param[in] symbol_table_sect_idx Index of the symbol table section in sect_head
 * @param[in] header Pointer to the ELF header containing class and endianness
 * @return int ELFPARSER_SUCCESS on success, ELFPARSER_ERR_NULL if inputs are NULL,
 *             ELFPARSER_ERR_RANGE if symbol_table_sect_idx is invalid,
 *             ELFPARSER_ERR_NOT_FOUND if string table not found,
 *             ELFPARSER_ERR_MALLOC if memory allocation fails
 */
int ElfParser_SymTable_structSetup(elfparser_symtable_t *symbol_table, const elfparser_secthead_t *sect_head, uint16_t symbol_table_sect_idx, const elfparser_header_t *header)
{
    if (!symbol_table || !sect_head || !header)
    {
        return ELFPARSER_ERR_NULL;  // Null pointer input
    }
    if (symbol_table_sect_idx >= sect_head->table_len || !sect_head->table)
    {
        return ELFPARSER_ERR_RANGE;  // Invalid section index
    }

    symbol_table->elf_class = header->elf_ident.elf_class;  // Set ELF class
    symbol_table->elf_data = header->elf_ident.elf_data;    // Set endianness
    symbol_table->entry_size = sect_head->table[symbol_table_sect_idx].sh_entsize; // Size of each entry
    symbol_table->table_len = sect_head->table[symbol_table_sect_idx].sh_size / symbol_table->entry_size; // Number of entries
    int32_t temp = ElfParser_SectHead_byNameFind(sect_head, SYMTABLE_STRING_SECT_NAME, 0); // Find string table
    if (temp == ELFPARSER_ERR_NOT_FOUND)
    {
        return ELFPARSER_ERR_NOT_FOUND;  // String table not found
    }
    if (temp < 0)  // Other errors from SectHead_byNameFind
    {
        return temp;  // Propagate error
    }
    symbol_table->string_table_idx = (uint16_t)temp;  // Set string table index
    symbol_table->max_idx = 0;                        // Initialize max name index
    symbol_table->table = malloc(symbol_table->table_len * sizeof(elfparser_symtable_entry_t)); // Allocate table
    if (!symbol_table->table)
    {
        return ELFPARSER_ERR_MALLOC;  // Allocation failure
    }
    return ELFPARSER_SUCCESS;  // Success
}

/**
 * @brief Parses the symbol table from a memory map
 * @param[out] symbol_table Pointer to the symbol table structure to populate
 * @param[in] map Pointer to the memory-mapped ELF file
 * @param[in] map_size Size of the memory map in bytes
 * @return int ELFPARSER_SUCCESS on success, ELFPARSER_ERR_NULL if inputs are NULL,
 *             ELFPARSER_ERR_SIZE if size is insufficient, ELFPARSER_ERR_CLASS if class or endianness is invalid
 */
int ElfParser_SymTable_parse(elfparser_symtable_t *symbol_table, const void *map, size_t map_size)
{
    if (!symbol_table || !map)
    {
        return ELFPARSER_ERR_NULL;  // Null pointer input
    }
    size_t required_size = (size_t)symbol_table->entry_size * symbol_table->table_len;
    if (map_size < required_size || required_size == 0)
    {
        return ELFPARSER_ERR_SIZE;  // Insufficient size or invalid length
    }

    const uint64_t mem_off_32bit[] = { SYMTABLE_ENTRY_NAMEIDX_OFF, SYMTABLE_ENTRY_INFO_OFF_32BIT, SYMTABLE_ENTRY_OTHER_OFF_32BIT, // 32-bit offsets
                                       SYMTABLE_ENTRY_SECTIDX_OFF_32BIT, SYMTABLE_ENTRY_VALUE_OFF_32BIT, SYMTABLE_ENTRY_SIZE_OFF_32BIT };
    const uint64_t mem_off_64bit[] = { SYMTABLE_ENTRY_NAMEIDX_OFF, SYMTABLE_ENTRY_INFO_OFF_64BIT, SYMTABLE_ENTRY_OTHER_OFF_64BIT, // 64-bit offsets
                                       SYMTABLE_ENTRY_SECTIDX_OFF_64BIT, SYMTABLE_ENTRY_VALUE_OFF_64BIT, SYMTABLE_ENTRY_SIZE_OFF_64BIT };
    const uint64_t mem_size_32bit[] = { SYMTABLE_ENTRY_NAMEIDX_SIZE, SYMTABLE_ENTRY_INFO_SIZE, SYMTABLE_ENTRY_OTHER_SIZE, // 32-bit sizes
                                        SYMTABLE_ENTRY_SECTIDX_SIZE, SYMTABLE_ENTRY_VALUE_SIZE_32BIT, SYMTABLE_ENTRY_SIZE_SIZE_32BIT };
    const uint64_t mem_size_64bit[] = { SYMTABLE_ENTRY_NAMEIDX_SIZE, SYMTABLE_ENTRY_INFO_SIZE, SYMTABLE_ENTRY_OTHER_SIZE, // 64-bit sizes
                                        SYMTABLE_ENTRY_SECTIDX_SIZE, SYMTABLE_ENTRY_VALUE_SIZE_64BIT, SYMTABLE_ENTRY_SIZE_SIZE_64BIT };

    const uint64_t *mem_off, *mem_size;  // Selected offset and size arrays
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
        return ELFPARSER_ERR_CLASS;  // Invalid class
    }

    for (uint32_t i = 0; i < symbol_table->table_len; i++)  // Parse each symbol entry
    {
        void *const ret_dest[SYMTABLE_ENTRY_LEN] = { // Static destination array
            &(symbol_table->table[i].sym_name_idx), &(symbol_table->table[i].sym_type),
            &(symbol_table->table[i].sym_visibility), &(symbol_table->table[i].sym_sect_idx),
            &(symbol_table->table[i].sym_value), &(symbol_table->table[i].sym_size)
        };
        symbol_table->table[i].sym_name = NULL;  // Initialize name pointer only

        size_t base_offset = i * symbol_table->entry_size;
        for (uint8_t j = 0; j < SYMTABLE_ENTRY_LEN; j++)  // Copy fields with bounds check
        {
            size_t offset = base_offset + mem_off[j];
            if (offset + mem_size[j] > map_size)
            {
                return ELFPARSER_ERR_SIZE;  // Out of bounds
            }
            if (symbol_table->elf_data == ELFPARSER_HEADER_DATA_BIG_ENDIANNESS)
            {
                ElfParser_memRevCpy(ret_dest[j], map + offset, mem_size[j]);
            }
            else if (symbol_table->elf_data == ELFPARSER_HEADER_DATA_LITTLE_ENDIANNESS)
            {
                ElfParser_memCpy(ret_dest[j], map + offset, mem_size[j]);
            }
            else
            {
                return ELFPARSER_ERR_CLASS;  // Invalid endianness
            }
        }
        if (symbol_table->table[i].sym_name_idx > symbol_table->max_idx)
        {
            symbol_table->max_idx = symbol_table->table[i].sym_name_idx;  // Update max name index
        }
        symbol_table->table[i].sym_bind = symbol_table->table[i].sym_type >> 4u;  // Extract binding from st_info
        symbol_table->table[i].sym_type &= 0x0f;  // Mask to get type from st_info
    }
    return ELFPARSER_SUCCESS;  // Success
}

/**
 * @brief Resolves symbol names from the string table
 * @param[in] symbol_table Pointer to the symbol table structure
 * @param[in] map Pointer to the memory-mapped ELF file
 * @param[in] map_size Size of the memory map in bytes
 * @return int ELFPARSER_SUCCESS on success, ELFPARSER_ERR_NULL if inputs are NULL,
 *             ELFPARSER_ERR_SIZE if map is too small, ELFPARSER_ERR_MALLOC if string duplication fails
 */
int ElfParser_SymTable_nameResolve(const elfparser_symtable_t *symbol_table, const void *map, size_t map_size)
{
    if (!symbol_table || !map)
    {
        return ELFPARSER_ERR_NULL;  // Null pointer input
    }
    if (map_size <= symbol_table->max_idx)  // Check if map covers max index
    {
        return ELFPARSER_ERR_SIZE;  // Insufficient size
    }

    const char *char_map = map;  // Cast map to char pointer
    for (size_t cnt = 0; cnt < symbol_table->table_len; cnt++)  // Resolve names
    {
        size_t name_idx = symbol_table->table[cnt].sym_name_idx;
        if (name_idx >= map_size)  // Bounds check
        {
            return ELFPARSER_ERR_SIZE;
        }
        if (ElfParser_strDup(&char_map[name_idx], &(symbol_table->table[cnt].sym_name)) < 0)
        {
            return ELFPARSER_ERR_MALLOC;  // String duplication failed
        }
    }
    return ELFPARSER_SUCCESS;  // Success
}

/**
 * @brief Frees the symbol table structure and its allocated resources
 * @param[in,out] symbol_table Pointer to the symbol table structure to free
 * @return int ELFPARSER_SUCCESS on success, ELFPARSER_ERR_NULL if symbol_table or table is NULL
 */
int ElfParser_SymTable_free(elfparser_symtable_t *symbol_table)
{
    if (!symbol_table || !symbol_table->table)
    {
        return ELFPARSER_ERR_NULL;  // Null pointer input
    }
    for (size_t cnt = 0; cnt < symbol_table->table_len; cnt++)  // Free each symbol name
    {
        free(symbol_table->table[cnt].sym_name);  // Safe to free NULL
        symbol_table->table[cnt].sym_name = NULL; // Ensure NULL after free
    }
    free(symbol_table->table);  // Free the table
    symbol_table->table = NULL; // Nullify pointer
    return ELFPARSER_SUCCESS;   // Success
}

/**
 * @brief Finds a symbol by name
 * @param[in] symbol_table Pointer to the symbol table structure
 * @param[in] name Name of the symbol to find
 * @param[in] start_idx Starting index for the search
 * @return int32_t Index of the found symbol, ELFPARSER_SYMTABLE_NOT_FOUND if not found,
 *                 ELFPARSER_ERR_NULL if inputs are NULL, ELFPARSER_ERR_RANGE if table or start_idx is invalid
 */
int32_t ElfParser_SymTable_byNameFind(const elfparser_symtable_t *symbol_table, const char *name, size_t start_idx)
{
    if (!symbol_table || !name)
    {
        return ELFPARSER_ERR_NULL;  // Null pointer input
    }
    if (!symbol_table->table || symbol_table->table_len <= start_idx)
    {
        return ELFPARSER_ERR_RANGE;  // Invalid table or start index
    }
    for (size_t cnt = start_idx; cnt < symbol_table->table_len; cnt++)  // Search for match
    {
        if (ElfParser_strCmp(symbol_table->table[cnt].sym_name, name) == 0)
        {
            return (int32_t)cnt;  // Return index
        }
    }
    return ELFPARSER_ERR_NOT_FOUND;  // Not found (assumes defined in elfparser_symtable.h)
}