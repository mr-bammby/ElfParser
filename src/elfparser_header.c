/**
 * @file elfparser_header.c
 * @brief ELF file header parsing functions for libelfparser
 * @author Domen Banfi
 * @date 2025-03-16
 * @version 1.0
 *
 * This file implements functions to parse ELF file headers, including the
 * identification block and main header fields, for use in the standalone
 * libelfparser library. It supports both 32-bit and 64-bit ELF formats and
 * handles endianness variations with robust error checking.
 */

#include "../inc_pub/elfparser_header.h"
#include "../inc_priv/elfparser_header_priv.h"
#include "../inc_priv/elfparser_memmanip_priv.h"

#define ELFPARSER_MAGIC_WORD "\177ELF" /**< ELF magic number (0x7F followed by "ELF") */

/**
 * @brief Parses the ELF identification block from a memory map
 * @param[out] elf_header Pointer to the ELF header structure to populate
 * @param[in] map Pointer to the memory-mapped ELF file
 * @param[in] size Size of the memory map in bytes
 * @return int ELFPARSER_SUCCESS on success, ELFPARSER_ERR_SIZE if size is too small,
 *             ELFPARSER_ERR_NULL if inputs are NULL, ELFPARSER_ERR_FORMAT if magic number is invalid
 */
int ElfParser_Header_identParse(elfparser_header_t *elf_header, const void *map, size_t size)
{
    const char expected_magic_word[] = ELFPARSER_MAGIC_WORD;  // Expected ELF magic number
    const uint32_t mem_off[] = { IDENT_MAGIC_OFF,             // Offsets for ident fields
                                 IDENT_CLASS_OFF,
                                 IDENT_DATA_OFF,
                                 IDENT_VERSION_OFF,
                                 IDENT_OSABI_OFF,
                                 IDENT_ABIVERSION_OFF };
    const uint32_t mem_size[] = { IDENT_MAGIC_SIZE,           // Sizes for ident fields
                                  IDENT_CLASS_SIZE,
                                  IDENT_DATA_SIZE,
                                  IDENT_VERSION_SIZE,
                                  IDENT_OSABI_SIZE,
                                  IDENT_ABIVERSION_SIZE };
    void *const ret_dest[] = { &(elf_header->elf_ident.elf_magic_num),  // Destinations for ident fields
                               &(elf_header->elf_ident.elf_class),
                               &(elf_header->elf_ident.elf_data),
                               &(elf_header->elf_ident.elf_version),
                               &(elf_header->elf_ident.elf_osabi),
                               &(elf_header->elf_ident.elf_abi_version) };

    if (size < 16)  // Minimum size for ELF ident (EI_NIDENT is 16 bytes)
    {
        return ELFPARSER_ERR_SIZE;  // Insufficient size
    }
    if (!elf_header || !map)
    {
        return ELFPARSER_ERR_NULL;  // Null pointer input
    }
    if (ElfParser_memCmp(map, expected_magic_word, 4) != 0)  // Validate magic number
    {
        return ELFPARSER_ERR_FORMAT;  // Invalid ELF file
    }

    for (uint8_t i = 0; i < IDENT_ENTRY_NUM; i++)  // Copy ident fields
    {
        if (mem_off[i] + mem_size[i] > size)  // Bounds check
        {
            return ELFPARSER_ERR_SIZE; // Invalid size
        }
        ElfParser_memCpy(ret_dest[i], map + mem_off[i], mem_size[i]);
    }
    return ELFPARSER_SUCCESS;  // Success
}

/**
 * @brief Retrieves the size of the ELF header based on its class
 * @param[in] elf_header Pointer to the ELF header structure
 * @return size_t Size of the header in bytes, or 0 if elf_header is NULL or class is invalid
 */
size_t ElfParser_Header_sizeGet(const elfparser_header_t *elf_header)
{
    if (!elf_header)
    {
        return 0;  // Null pointer input
    }
    return (elf_header->elf_ident.elf_class == ELFPARSER_HEADER_CLASS_32_BIT) ? ELFPARSER_HEADER_SIZE_32BIT :       // if class 32 BIT
           (elf_header->elf_ident.elf_class == ELFPARSER_HEADER_CLASS_64_BIT) ? ELFPARSER_HEADER_SIZE_64BIT : 0;    // if class 64 BIT 
}                                                                                                                   // else invalid 

/**
 * @brief Parses the full ELF header from a memory map
 * @param[out] elf_header Pointer to the ELF header structure to populate
 * @param[in] map Pointer to the memory-mapped ELF file
 * @param[in] size Size of the memory map in bytes
 * @return int ELFPARSER_SUCCESS on success, ELFPARSER_ERR_SIZE if size is insufficient,
 *             ELFPARSER_ERR_NULL if inputs are NULL, ELFPARSER_ERR_CLASS if class or endianness is invalid
 */
int ElfParser_Header_parse(elfparser_header_t *elf_header, const void *map, size_t size)
{
    const uint32_t mem_off_32bit[] = {  HEADER_TYPE_OFF,
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
                                        HEADER_SECTTENTNAMEIDX_OFF_32BIT };

    const uint32_t mem_off_64bit[] = {  HEADER_TYPE_OFF,
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
                                        HEADER_SECTTENTNAMEIDX_OFF_64BIT };

    const uint32_t mem_size_32bit[] = { HEADER_TYPE_SIZE,
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
                                        HEADER_SECTTENTNAMEIDX_SIZE };

    const uint32_t mem_size_64bit[] = { HEADER_TYPE_SIZE,
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
                                        HEADER_SECTTENTNAMEIDX_SIZE };

    void *const ret_dest[] = { &(elf_header->elf_type), &(elf_header->elf_machine), &(elf_header->elf_version),
                               &(elf_header->elf_entry), &(elf_header->elf_program_header_off), &(elf_header->elf_section_header_off),
                               &(elf_header->elf_flags), &(elf_header->elf_header_size), &(elf_header->elf_program_header_entry_size),
                               &(elf_header->elf_program_header_entry_num), &(elf_header->elf_section_header_entry_size),
                               &(elf_header->elf_section_header_entry_num), &(elf_header->elf_section_header_name_idx) };

    if (!elf_header || !map)
    {
        return ELFPARSER_ERR_NULL;  // Null pointer input
    }
    size_t header_size = ElfParser_Header_sizeGet(elf_header);  // Cache header size
    if (size < header_size || header_size == 0)
    {
        return ELFPARSER_ERR_SIZE;  // Insufficient size or invalid class
    }

    const uint32_t *mem_off;    // Selected offset array
    const uint32_t *mem_size;   // Selected size array
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
        return ELFPARSER_ERR_CLASS;  // Invalid class
    }

    for (uint8_t i = 0; i < HEADER_ENTRY_NUM; i++)  // Parse header fields
    {
        if (mem_off[i] + mem_size[i] > size)  // Bounds check
        {
            return ELFPARSER_ERR_SIZE;
        }
        if (elf_header->elf_ident.elf_data == ELFPARSER_HEADER_DATA_BIG_ENDIANNESS)
        {
            ElfParser_memRevCpy(ret_dest[i], map + mem_off[i], mem_size[i]);  // Big-endian copy
        }
        else if (elf_header->elf_ident.elf_data == ELFPARSER_HEADER_DATA_LITTLE_ENDIANNESS)
        {
            ElfParser_memCpy(ret_dest[i], map + mem_off[i], mem_size[i]);  // Little-endian copy
        }
        else
        {
            return ELFPARSER_ERR_CLASS;  // Invalid endianness
        }
    }
    return ELFPARSER_SUCCESS;  // Success
}
