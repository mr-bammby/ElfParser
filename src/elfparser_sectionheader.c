/**
 * @file elfparser_secthead.c
 * @brief ELF section header parsing functions for libelfparser
 * @author Domen Banfi
 * @date 2025-03-16
 * @version 1.0
 *
 * This file implements functions to parse and manage ELF section headers within
 * the standalone libelfparser library. It supports setting up section header
 * structures, parsing section header tables, resolving section names, and
 * freeing allocated resources, for both 32-bit and 64-bit ELF formats.
 */

#include "../inc_priv/elfparser_secthead_priv.h"
#include "../inc_pub/elfparser_secthead.h"
#include "../inc_priv/elfparser_memmanip_priv.h"
#include "../inc_pub/elfparser_header.h"
#include <stdlib.h>

/**
 * @brief Sets up the section header structure using ELF header data
 * @param[out] sect_head Pointer to the section header structure to initialize
 * @param[in] header Pointer to the ELF header containing section metadata
 * @return int ELFPARSER_SUCCESS on success, ELFPARSER_ERR_NULL if inputs are NULL,
 *             ELFPARSER_ERR_MALLOC if memory allocation fails
 */
int ElfParser_SectHead_structSetup(elfparser_secthead_t *sect_head, const elfparser_header_t *header)
{
    if (!sect_head || !header)
    {
        return ELFPARSER_ERR_NULL;  // Null pointer input
    }

    sect_head->elf_class = header->elf_ident.elf_class;                 // Set ELF class (32/64-bit)
    sect_head->elf_data = header->elf_ident.elf_data;                   // Set endianness
    sect_head->entry_size = header->elf_section_header_entry_size;      // Size of each section header entry
    sect_head->table_len = header->elf_section_header_entry_num;        // Number of section header entries
    sect_head->string_table_idx = header->elf_section_header_name_idx;  // Index of string table section
    sect_head->max_idx = 0;                                             // Initialize max name index
    sect_head->table = malloc(sect_head->table_len * sizeof(elfparser_secthead_entry_t)); // Allocate table
    if (!sect_head->table)
    {
        return ELFPARSER_ERR_MALLOC;  // Allocation failure
    }
    return ELFPARSER_SUCCESS;  // Success
}

/**
 * @brief Parses the section header table from a memory map
 * @param[out] sect_head Pointer to the section header structure to populate
 * @param[in] map Pointer to the memory-mapped ELF file
 * @param[in] map_size Size of the memory map in bytes
 * @return int ELFPARSER_SUCCESS on success, ELFPARSER_ERR_NULL if inputs are NULL,
 *             ELFPARSER_ERR_SIZE if size is insufficient, ELFPARSER_ERR_CLASS if class or endianness is invalid
 */
int ElfParser_SectHead_parse(elfparser_secthead_t *sect_head, const void *map, size_t map_size)
{
    if (!sect_head || !map)
    {
        return ELFPARSER_ERR_NULL;  // Null pointer input
    }
    size_t required_size = (size_t)sect_head->entry_size * sect_head->table_len;
    if (map_size < required_size || required_size == 0)
    {
        return ELFPARSER_ERR_SIZE;  // Insufficient size or invalid table length
    }

    const uint64_t mem_off_32bit[] = { SECTHEADER_ENTRY_NAMEIDX_OFF, SECTHEADER_ENTRY_TYPE_OFF, SECTHEADER_ENTRY_FLAGS_OFF, // 32-bit offsets
                                       SECTHEADER_ENTRY_SECTADDR_OFF_32BIT, SECTHEADER_ENTRY_SECTOFF_OFF_32BIT, SECTHEADER_ENTRY_SECTSIZE_OFF_32BIT,
                                       SECTHEADER_ENTRY_LINK_OFF_32BIT, SECTHEADER_ENTRY_INFO_OFF_32BIT, SECTHEADER_ENTRY_ADDRALIGN_OFF_32BIT,
                                       SECTHEADER_ENTRY_ENTRYSIZE_OFF_32BIT };
    const uint64_t mem_off_64bit[] = { SECTHEADER_ENTRY_NAMEIDX_OFF, SECTHEADER_ENTRY_TYPE_OFF, SECTHEADER_ENTRY_FLAGS_OFF, // 64-bit offsets
                                       SECTHEADER_ENTRY_SECTADDR_OFF_64BIT, SECTHEADER_ENTRY_SECTOFF_OFF_64BIT, SECTHEADER_ENTRY_SECTSIZE_OFF_64BIT,
                                       SECTHEADER_ENTRY_LINK_OFF_64BIT, SECTHEADER_ENTRY_INFO_OFF_64BIT, SECTHEADER_ENTRY_ADDRALIGN_OFF_64BIT,
                                       SECTHEADER_ENTRY_ENTRYSIZE_OFF_64BIT };
    const uint64_t mem_size_32bit[] = { SECTHEADER_ENTRY_NAMEIDX_SIZE, SECTHEADER_ENTRY_TYPE_SIZE, SECTHEADER_ENTRY_FLAGS_SIZE_32BIT, // 32-bit sizes
                                        SECTHEADER_ENTRY_SECTADDR_SIZE_32BIT, SECTHEADER_ENTRY_SECTOFF_SIZE_32BIT, SECTHEADER_ENTRY_SECTSIZE_SIZE_32BIT,
                                        SECTHEADER_ENTRY_LINK_SIZE, SECTHEADER_ENTRY_INFO_SIZE, SECTHEADER_ENTRY_ADDRALIGN_SIZE_32BIT,
                                        SECTHEADER_ENTRY_ENTRYSIZE_SIZE_32BIT };
    const uint64_t mem_size_64bit[] = { SECTHEADER_ENTRY_NAMEIDX_SIZE, SECTHEADER_ENTRY_TYPE_SIZE, SECTHEADER_ENTRY_FLAGS_SIZE_64BIT, // 64-bit sizes
                                        SECTHEADER_ENTRY_SECTADDR_SIZE_64BIT, SECTHEADER_ENTRY_SECTOFF_SIZE_64BIT, SECTHEADER_ENTRY_SECTSIZE_SIZE_64BIT,
                                        SECTHEADER_ENTRY_LINK_SIZE, SECTHEADER_ENTRY_INFO_SIZE, SECTHEADER_ENTRY_ADDRALIGN_SIZE_64BIT,
                                        SECTHEADER_ENTRY_ENTRYSIZE_SIZE_64BIT };

    const uint64_t *mem_off, *mem_size;  // Selected offset and size arrays
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
        return ELFPARSER_ERR_CLASS;  // Invalid class
    }

    for (uint32_t i = 0; i < sect_head->table_len; i++)  // Parse each section header entry
    {
        void *const ret_dest[SECTHEADER_ENTRY_LEN] = { // Static destination array
            &(sect_head->table[i].sh_name_idx), &(sect_head->table[i].sh_type), &(sect_head->table[i].sh_flags),
            &(sect_head->table[i].sh_addr), &(sect_head->table[i].sh_offset), &(sect_head->table[i].sh_size),
            &(sect_head->table[i].sh_link), &(sect_head->table[i].sh_info), &(sect_head->table[i].sh_addralign),
            &(sect_head->table[i].sh_entsize)
        };
        sect_head->table[i].sh_name = NULL;  // Initialize name pointer only

        size_t base_offset = i * sect_head->entry_size;
        for (uint8_t j = 0; j < SECTHEADER_ENTRY_LEN; j++)  // Copy fields with bounds check
        {
            size_t offset = base_offset + mem_off[j];
            if (offset + mem_size[j] > map_size)
            {
                return ELFPARSER_ERR_SIZE;  // Out of bounds
            }
            if (sect_head->elf_data == ELFPARSER_HEADER_DATA_BIG_ENDIANNESS)
            {
                ElfParser_memRevCpy(ret_dest[j], map + offset, mem_size[j]);
            }
            else if (sect_head->elf_data == ELFPARSER_HEADER_DATA_LITTLE_ENDIANNESS)
            {
                ElfParser_memCpy(ret_dest[j], map + offset, mem_size[j]);
            }
            else
            {
                return ELFPARSER_ERR_CLASS;  // Invalid endianness
            }
        }
        if (sect_head->table[i].sh_name_idx > sect_head->max_idx)
        {
            sect_head->max_idx = sect_head->table[i].sh_name_idx;  // Update max name index
        }
    }
    return ELFPARSER_SUCCESS;  // Success
}

/**
 * @brief Resolves section names from the string table
 * @param[in] sect_head Pointer to the section header structure
 * @param[in] map Pointer to the memory-mapped ELF file
 * @param[in] map_size Size of the memory map in bytes
 * @return int ELFPARSER_SUCCESS on success, ELFPARSER_ERR_SIZE if map is too small,
 *             ELFPARSER_ERR_NULL if inputs are NULL, ELFPARSER_ERR_MALLOC if string duplication fails
 */
int ElfParser_SectHead_nameResolve(const elfparser_secthead_t *sect_head, const void *map, size_t map_size)
{
    if (!sect_head || !map)
    {
        return ELFPARSER_ERR_NULL;  // Null pointer input
    }
    if (map_size <= sect_head->max_idx)  // Check if map covers max index
    {
        return ELFPARSER_ERR_SIZE;  // Insufficient size
    }

    const char *char_map = map;  // Cast map to char pointer
    for (size_t cnt = 0; cnt < sect_head->table_len; cnt++)  // Resolve names
    {
        size_t name_idx = sect_head->table[cnt].sh_name_idx;
        if (name_idx >= map_size)  // Bounds check
        {
            return ELFPARSER_ERR_SIZE;
        }
        if (ElfParser_strDup(&char_map[name_idx], &(sect_head->table[cnt].sh_name)) < 0)
        {
            return ELFPARSER_ERR_MALLOC;  // String duplication failed
        }
    }
    return ELFPARSER_SUCCESS;  // Success
}

/**
 * @brief Frees the section header structure and its allocated resources
 * @param[in,out] sect_head Pointer to the section header structure to free
 * @return int ELFPARSER_SUCCESS on success, ELFPARSER_ERR_NULL if sect_head or table is NULL
 */
int ElfParser_SectHead_free(elfparser_secthead_t *sect_head)
{
    if (!sect_head || !sect_head->table)
    {
        return ELFPARSER_ERR_NULL;  // Null pointer input
    }
    for (size_t cnt = 0; cnt < sect_head->table_len; cnt++)  // Free each section name
    {
        free(sect_head->table[cnt].sh_name);  // Safe to free NULL
        sect_head->table[cnt].sh_name = NULL; // Optional: ensure NULL after free
    }
    free(sect_head->table);  // Free the table
    sect_head->table = NULL; // Nullify pointer
    return ELFPARSER_SUCCESS;  // Success
}

/**
 * @brief Finds a section header by name
 * @param[in] sect_head Pointer to the section header structure
 * @param[in] name Name of the section to find
 * @param[in] start_idx Starting index for the search
 * @return int32_t Index of the found section, ELFPARSER_ERR_NULL if inputs are NULL,
 *                 ELFPARSER_ERR_RANGE if table is invalid or start_idx is out of bounds,
 *                 ELFPARSER_SECTHEAD_NOT_FOUND if not found
 */
int32_t ElfParser_SectHead_byNameFind(const elfparser_secthead_t *sect_head, const char *name, size_t start_idx)
{
    if (!sect_head || !name)
    {
        return ELFPARSER_ERR_NULL;  // Null pointer input
    }
    if (!sect_head->table || sect_head->table_len <= start_idx)
    {
        return ELFPARSER_ERR_RANGE;  // Invalid table or start index
    }
    for (size_t cnt = start_idx; cnt < sect_head->table_len; cnt++)  // Search for match
    {
        if (ElfParser_strCmp(sect_head->table[cnt].sh_name, name) == 0)
        {
            return (int32_t)cnt;  // Return index
        }
    }
    return ELFPARSER_ERR_NOT_FOUND;  // Not found
}
