/**
 * @file elfparser_secthead_priv.h
 * @brief Private header for ELF section header parsing constants in libelfparser
 * @author Domen Banfi
 * @date 2025-03-16
 * @version 1.0
 *
 * This header defines internal constants for parsing ELF section headers within
 * the standalone libelfparser library. It includes offsets and sizes for section
 * header fields, supporting both 32-bit and 64-bit formats. These constants are
 * used by elfparser_secthead.c and are not part of the public API.
 */

#ifndef _IG_ELFPARSER_SECTHEAD_PRIV_H_
#define _IG_ELFPARSER_SECTHEAD_PRIV_H_

#define SECTHEADER_ENTRY_LEN 10 /**< Number of fields in an ELF section header entry */

/* ELF Section Header Field Offsets (common to 32-bit and 64-bit where not specified) */
#define SECTHEADER_ENTRY_NAMEIDX_OFF        0x00u /**< Offset of section name index (sh_name) */
#define SECTHEADER_ENTRY_TYPE_OFF           0x04u /**< Offset of section type (sh_type) */
#define SECTHEADER_ENTRY_FLAGS_OFF          0x08u /**< Offset of section flags (sh_flags) */

/* 32-bit ELF Section Header Field Offsets */
#define SECTHEADER_ENTRY_SECTADDR_OFF_32BIT 0x0Cu /**< Offset of section address (sh_addr, 32-bit) */
#define SECTHEADER_ENTRY_SECTOFF_OFF_32BIT  0x10u /**< Offset of section offset (sh_offset, 32-bit) */
#define SECTHEADER_ENTRY_SECTSIZE_OFF_32BIT 0x14u /**< Offset of section size (sh_size, 32-bit) */
#define SECTHEADER_ENTRY_LINK_OFF_32BIT     0x18u /**< Offset of linked section index (sh_link, 32-bit) */
#define SECTHEADER_ENTRY_INFO_OFF_32BIT     0x1Cu /**< Offset of additional info (sh_info, 32-bit) */
#define SECTHEADER_ENTRY_ADDRALIGN_OFF_32BIT 0x20u /**< Offset of address alignment (sh_addralign, 32-bit) */
#define SECTHEADER_ENTRY_ENTRYSIZE_OFF_32BIT 0x24u /**< Offset of entry size (sh_entsize, 32-bit) */

/* 64-bit ELF Section Header Field Offsets */
#define SECTHEADER_ENTRY_SECTADDR_OFF_64BIT 0x10u /**< Offset of section address (sh_addr, 64-bit) */
#define SECTHEADER_ENTRY_SECTOFF_OFF_64BIT  0x18u /**< Offset of section offset (sh_offset, 64-bit) */
#define SECTHEADER_ENTRY_SECTSIZE_OFF_64BIT 0x20u /**< Offset of section size (sh_size, 64-bit) */
#define SECTHEADER_ENTRY_LINK_OFF_64BIT     0x28u /**< Offset of linked section index (sh_link, 64-bit) */
#define SECTHEADER_ENTRY_INFO_OFF_64BIT     0x2Cu /**< Offset of additional info (sh_info, 64-bit) */
#define SECTHEADER_ENTRY_ADDRALIGN_OFF_64BIT 0x30u /**< Offset of address alignment (sh_addralign, 64-bit) */
#define SECTHEADER_ENTRY_ENTRYSIZE_OFF_64BIT 0x38u /**< Offset of entry size (sh_entsize, 64-bit) */

/* ELF Section Header Field Sizes (common to 32-bit and 64-bit where not specified) */
#define SECTHEADER_ENTRY_NAMEIDX_SIZE       4u /**< Size of section name index (sh_name) */
#define SECTHEADER_ENTRY_TYPE_SIZE          4u /**< Size of section type (sh_type) */
#define SECTHEADER_ENTRY_FLAGS_SIZE_32BIT   4u /**< Size of section flags (sh_flags, 32-bit) */
#define SECTHEADER_ENTRY_FLAGS_SIZE_64BIT   8u /**< Size of section flags (sh_flags, 64-bit) */
#define SECTHEADER_ENTRY_SECTADDR_SIZE_32BIT 4u /**< Size of section address (sh_addr, 32-bit) */
#define SECTHEADER_ENTRY_SECTADDR_SIZE_64BIT 8u /**< Size of section address (sh_addr, 64-bit) */
#define SECTHEADER_ENTRY_SECTOFF_SIZE_32BIT 4u /**< Size of section offset (sh_offset, 32-bit) */
#define SECTHEADER_ENTRY_SECTOFF_SIZE_64BIT 8u /**< Size of section offset (sh_offset, 64-bit) */
#define SECTHEADER_ENTRY_SECTSIZE_SIZE_32BIT 4u /**< Size of section size (sh_size, 32-bit) */
#define SECTHEADER_ENTRY_SECTSIZE_SIZE_64BIT 8u /**< Size of section size (sh_size, 64-bit) */
#define SECTHEADER_ENTRY_LINK_SIZE          4u /**< Size of linked section index (sh_link) */
#define SECTHEADER_ENTRY_INFO_SIZE          4u /**< Size of additional info (sh_info) */
#define SECTHEADER_ENTRY_ADDRALIGN_SIZE_32BIT 4u /**< Size of address alignment (sh_addralign, 32-bit) */
#define SECTHEADER_ENTRY_ADDRALIGN_SIZE_64BIT 8u /**< Size of address alignment (sh_addralign, 64-bit) */
#define SECTHEADER_ENTRY_ENTRYSIZE_SIZE_32BIT 4u /**< Size of entry size (sh_entsize, 32-bit) */
#define SECTHEADER_ENTRY_ENTRYSIZE_SIZE_64BIT 8u /**< Size of entry size (sh_entsize, 64-bit) */

#endif /* _IG_ELFPARSER_SECTHEAD_PRIV_H_ */
