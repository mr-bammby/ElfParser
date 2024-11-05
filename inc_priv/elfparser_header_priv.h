#ifndef _IG_ELFPARSER_HEADER_PRIV_H_
#define _IG_ELFPARSER_HEADER_PRIV_H_

#define IDENT_ENTRY_NUM         6
#define HEADER_ENTRY_NUM        13

#define IDENT_MAGIC_OFF         0x0u
#define IDENT_CALSS_OFF         0x4u
#define IDENT_DATA_OFF          0x5u
#define IDENT_VERSION_OFF       0x6u
#define IDENT_OSABI_OFF         0x7u
#define IDENT_ABIVERSION_OFF    0x8u

#define IDENT_MAGIC_SIZE        4u
#define IDENT_CALSS_SIZE        1u
#define IDENT_DATA_SIZE         1u
#define IDENT_VERSION_SIZE      1u
#define IDENT_OSABI_SIZE        1u
#define IDENT_ABIVERSION_SIZE   1u

#define HEADER_TYPE_OFF                     0x10u
#define HEADER_MACHINE_OFF                  0x12u
#define HEADER_VERSION_OFF                  0x14u
#define HEADER_ENTRY_OFF                    0x18u
#define HEADER_PROGTABLEOFF_OFF_32BIT       0x1Cu
#define HEADER_PROGTABLEOFF_OFF_64BIT       0x20u
#define HEADER_SECTTABLEOFF_OFF_32BIT       0x20u
#define HEADER_SECTTABLEOFF_OFF_64BIT       0x28u
#define HEADER_FLAGS_OFF_32BIT              0x24u
#define HEADER_FLAGS_OFF_64BIT              0x30u
#define HEADER_HEADERSIZE_OFF_32BIT         0x28u
#define HEADER_HEADERSIZE_OFF_64BIT         0x34u
#define HEADER_PROGTENTSIZE_OFF_32BIT       0x2Au
#define HEADER_PROGTENTSIZE_OFF_64BIT       0x36u
#define HEADER_PROGTENTNUM_OFF_32BIT        0x2Cu
#define HEADER_PROGTENTNUM_OFF_64BIT        0x38u
#define HEADER_SECTTENTSIZE_OFF_32BIT       0x2Eu
#define HEADER_SECTTENTSIZE_OFF_64BIT       0x3Au
#define HEADER_SECTTENTNUM_OFF_32BIT        0x30u
#define HEADER_SECTTENTNUM_OFF_64BIT        0x3Cu
#define HEADER_SECTTENTNAMEIDX_OFF_32BIT    0x32u
#define HEADER_SECTTENTNAMEIDX_OFF_64BIT    0x3Eu


#define HEADER_TYPE_SIZE                    2u
#define HEADER_MACHINE_SIZE                 2u
#define HEADER_VERSION_SIZE                 4u
#define HEADER_ENTRY_SIZE_32BIT             4u
#define HEADER_ENTRY_SIZE_64BIT             8u
#define HEADER_PROGTABLEOFF_SIZE_32BIT      4u
#define HEADER_PROGTABLEOFF_SIZE_64BIT      8u
#define HEADER_SECTTABLEOFF_SIZE_32BIT      4u
#define HEADER_SECTTABLEOFF_SIZE_64BIT      8u
#define HEADER_FLAGS_SIZE                   4u
#define HEADER_HEADERSIZE_SIZE              2u
#define HEADER_PROGTENTSIZE_SIZE            2u
#define HEADER_PROGTENTNUM_SIZE             2u
#define HEADER_SECTTENTSIZE_SIZE            2u
#define HEADER_SECTTENTNUM_SIZE             2u
#define HEADER_SECTTENTNAMEIDX_SIZE         2u


#endif /* _IG_ELFPARSER_HEADER_PRIV_H_ */