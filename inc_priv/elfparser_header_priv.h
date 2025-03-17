/**
 * @file elfparser_header_priv.h
 * @brief Private header for ELF header parsing constants in libelfparser
 * @author Domen Banfi
 * @date 2025-03-16
 * @version 1.0
 *
 * This header defines internal constants for parsing ELF file headers within
 * the standalone libelfparser library. It includes offsets and sizes for the
 * ELF identification block and main header fields, supporting both 32-bit and
 * 64-bit formats. These constants are used by elfparser_header.c and are not
 * part of the public API.
 */

#ifndef _IG_ELFPARSER_HEADER_PRIV_H_
#define _IG_ELFPARSER_HEADER_PRIV_H_

#define IDENT_ENTRY_NUM         6   /**< Number of fields in the ELF identification block */
#define HEADER_ENTRY_NUM        13  /**< Number of fields in the ELF header (excluding ident) */

/* ELF Identification Block Offsets (EI_* fields per ELF specification) */
#define IDENT_MAGIC_OFF         0x0u /**< Offset of magic number (EI_MAG0 to EI_MAG3) */
#define IDENT_CLASS_OFF         0x4u /**< Offset of class (EI_CLASS) */
#define IDENT_DATA_OFF          0x5u /**< Offset of data encoding (EI_DATA) */
#define IDENT_VERSION_OFF       0x6u /**< Offset of file version (EI_VERSION) */
#define IDENT_OSABI_OFF         0x7u /**< Offset of OS/ABI identifier (EI_OSABI) */
#define IDENT_ABIVERSION_OFF    0x8u /**< Offset of ABI version (EI_ABIVERSION) */

/* ELF Identification Block Sizes */
#define IDENT_MAGIC_SIZE        4u   /**< Size of magic number (4 bytes) */
#define IDENT_CLASS_SIZE        1u   /**< Size of class (1 byte) */
#define IDENT_DATA_SIZE         1u   /**< Size of data encoding (1 byte) */
#define IDENT_VERSION_SIZE      1u   /**< Size of file version (1 byte) */
#define IDENT_OSABI_SIZE        1u   /**< Size of OS/ABI identifier (1 byte) */
#define IDENT_ABIVERSION_SIZE   1u   /**< Size of ABI version (1 byte) */

/* ELF Header Field Offsets (common to 32-bit and 64-bit where not specified) */
#define HEADER_TYPE_OFF                     0x10u /**< Offset of object file type (e_type) */
#define HEADER_MACHINE_OFF                  0x12u /**< Offset of target machine (e_machine) */
#define HEADER_VERSION_OFF                  0x14u /**< Offset of object file version (e_version) */
#define HEADER_ENTRY_OFF                    0x18u /**< Offset of entry point address (e_entry) */

/* 32-bit ELF Header Field Offsets */
#define HEADER_PROGTABLEOFF_OFF_32BIT       0x1Cu /**< Offset of program header table (e_phoff, 32-bit) */
#define HEADER_SECTTABLEOFF_OFF_32BIT       0x20u /**< Offset of section header table (e_shoff, 32-bit) */
#define HEADER_FLAGS_OFF_32BIT              0x24u /**< Offset of processor-specific flags (e_flags, 32-bit) */
#define HEADER_HEADERSIZE_OFF_32BIT         0x28u /**< Offset of header size (e_ehsize, 32-bit) */
#define HEADER_PROGTENTSIZE_OFF_32BIT       0x2Au /**< Offset of program header entry size (e_phentsize, 32-bit) */
#define HEADER_PROGTENTNUM_OFF_32BIT        0x2Cu /**< Offset of program header entry count (e_phnum, 32-bit) */
#define HEADER_SECTTENTSIZE_OFF_32BIT       0x2Eu /**< Offset of section header entry size (e_shentsize, 32-bit) */
#define HEADER_SECTTENTNUM_OFF_32BIT        0x30u /**< Offset of section header entry count (e_shnum, 32-bit) */
#define HEADER_SECTTENTNAMEIDX_OFF_32BIT    0x32u /**< Offset of section name string table index (e_shstrndx, 32-bit) */

/* 64-bit ELF Header Field Offsets */
#define HEADER_PROGTABLEOFF_OFF_64BIT       0x20u /**< Offset of program header table (e_phoff, 64-bit) */
#define HEADER_SECTTABLEOFF_OFF_64BIT       0x28u /**< Offset of section header table (e_shoff, 64-bit) */
#define HEADER_FLAGS_OFF_64BIT              0x30u /**< Offset of processor-specific flags (e_flags, 64-bit) */
#define HEADER_HEADERSIZE_OFF_64BIT         0x34u /**< Offset of header size (e_ehsize, 64-bit) */
#define HEADER_PROGTENTSIZE_OFF_64BIT       0x36u /**< Offset of program header entry size (e_phentsize, 64-bit) */
#define HEADER_PROGTENTNUM_OFF_64BIT        0x38u /**< Offset of program header entry count (e_phnum, 64-bit) */
#define HEADER_SECTTENTSIZE_OFF_64BIT       0x3Au /**< Offset of section header entry size (e_shentsize, 64-bit) */
#define HEADER_SECTTENTNUM_OFF_64BIT        0x3Cu /**< Offset of section header entry count (e_shnum, 64-bit) */
#define HEADER_SECTTENTNAMEIDX_OFF_64BIT    0x3Eu /**< Offset of section name string table index (e_shstrndx, 64-bit) */

/* ELF Header Field Sizes (common to 32-bit and 64-bit where not specified) */
#define HEADER_TYPE_SIZE                    2u   /**< Size of object file type (e_type) */
#define HEADER_MACHINE_SIZE                 2u   /**< Size of target machine (e_machine) */
#define HEADER_VERSION_SIZE                 4u   /**< Size of object file version (e_version) */
#define HEADER_ENTRY_SIZE_32BIT             4u   /**< Size of entry point address (e_entry, 32-bit) */
#define HEADER_ENTRY_SIZE_64BIT             8u   /**< Size of entry point address (e_entry, 64-bit) */
#define HEADER_PROGTABLEOFF_SIZE_32BIT      4u   /**< Size of program header table offset (e_phoff, 32-bit) */
#define HEADER_PROGTABLEOFF_SIZE_64BIT      8u   /**< Size of program header table offset (e_phoff, 64-bit) */
#define HEADER_SECTTABLEOFF_SIZE_32BIT      4u   /**< Size of section header table offset (e_shoff, 32-bit) */
#define HEADER_SECTTABLEOFF_SIZE_64BIT      8u   /**< Size of section header table offset (e_shoff, 64-bit) */
#define HEADER_FLAGS_SIZE                   4u   /**< Size of processor-specific flags (e_flags) */
#define HEADER_HEADERSIZE_SIZE              2u   /**< Size of header size (e_ehsize) */
#define HEADER_PROGTENTSIZE_SIZE            2u   /**< Size of program header entry size (e_phentsize) */
#define HEADER_PROGTENTNUM_SIZE             2u   /**< Size of program header entry count (e_phnum) */
#define HEADER_SECTTENTSIZE_SIZE            2u   /**< Size of section header entry size (e_shentsize) */
#define HEADER_SECTTENTNUM_SIZE             2u   /**< Size of section header entry count (e_shnum) */
#define HEADER_SECTTENTNAMEIDX_SIZE         2u   /**< Size of section name string table index (e_shstrndx) */

#endif /* _IG_ELFPARSER_HEADER_PRIV_H_ */