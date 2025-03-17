/**
 * @file elfparser_symtable_priv.h
 * @brief Private header for ELF symbol table parsing constants in libelfparser
 * @author Domen Banfi
 * @date 2025-03-16
 * @version 1.0
 *
 * This header defines internal constants for parsing ELF symbol tables within
 * the standalone libelfparser library. It includes offsets and sizes for symbol
 * table fields, supporting both 32-bit and 64-bit formats, as well as the name
 * of the string table section. These constants are used by elfparser_symtable.c
 * and are not part of the public API.
 */

#ifndef _IG_ELFPARSER_SYMTABLE_PRIV_H_
#define _IG_ELFPARSER_SYMTABLE_PRIV_H_

#define SYMTABLE_ENTRY_LEN 6 /**< Number of fields in an ELF symbol table entry */

/* ELF Symbol Table Field Offsets */
#define SYMTABLE_ENTRY_NAMEIDX_OFF      0x00u /**< Offset of symbol name index (st_name) */

/* 32-bit ELF Symbol Table Field Offsets */
#define SYMTABLE_ENTRY_INFO_OFF_32BIT   0x0Cu /**< Offset of symbol info (st_info, 32-bit) */
#define SYMTABLE_ENTRY_OTHER_OFF_32BIT  0x0Du /**< Offset of symbol visibility (st_other, 32-bit) */
#define SYMTABLE_ENTRY_SECTIDX_OFF_32BIT 0x0Eu /**< Offset of section index (st_shndx, 32-bit) */
#define SYMTABLE_ENTRY_VALUE_OFF_32BIT  0x04u /**< Offset of symbol value (st_value, 32-bit) */
#define SYMTABLE_ENTRY_SIZE_OFF_32BIT   0x08u /**< Offset of symbol size (st_size, 32-bit) */

/* 64-bit ELF Symbol Table Field Offsets */
#define SYMTABLE_ENTRY_INFO_OFF_64BIT   0x04u /**< Offset of symbol info (st_info, 64-bit) */
#define SYMTABLE_ENTRY_OTHER_OFF_64BIT  0x05u /**< Offset of symbol visibility (st_other, 64-bit) */
#define SYMTABLE_ENTRY_SECTIDX_OFF_64BIT 0x06u /**< Offset of section index (st_shndx, 64-bit) */
#define SYMTABLE_ENTRY_VALUE_OFF_64BIT  0x08u /**< Offset of symbol value (st_value, 64-bit) */
#define SYMTABLE_ENTRY_SIZE_OFF_64BIT   0x10u /**< Offset of symbol size (st_size, 64-bit) */

/* ELF Symbol Table Field Sizes */
#define SYMTABLE_ENTRY_NAMEIDX_SIZE     4u /**< Size of symbol name index (st_name) */
#define SYMTABLE_ENTRY_INFO_SIZE        1u /**< Size of symbol info (st_info) */
#define SYMTABLE_ENTRY_OTHER_SIZE       1u /**< Size of symbol visibility (st_other) */
#define SYMTABLE_ENTRY_SECTIDX_SIZE     2u /**< Size of section index (st_shndx) */
#define SYMTABLE_ENTRY_VALUE_SIZE_32BIT 4u /**< Size of symbol value (st_value, 32-bit) */
#define SYMTABLE_ENTRY_VALUE_SIZE_64BIT 8u /**< Size of symbol value (st_value, 64-bit) */
#define SYMTABLE_ENTRY_SIZE_SIZE_32BIT  4u /**< Size of symbol size (st_size, 32-bit) */
#define SYMTABLE_ENTRY_SIZE_SIZE_64BIT  8u /**< Size of symbol size (st_size, 64-bit) */

/* String Table Section Name */
#define SYMTABLE_STRING_SECT_NAME ".strtab" /**< Name of the string table section containing symbol names */

#endif /* _IG_ELFPARSER_SYMTABLE_PRIV_H_ */