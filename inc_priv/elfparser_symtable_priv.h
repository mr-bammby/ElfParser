#ifndef _IG_ELFPARSER_SYMTABLE_PRIV_H_
#define _IG_ELFPARSER_SYMTABLE_PRIV_H_

#define SECTHEADER_ENTRY_LEN 7


#define SYMTABLE_ENTRY_NAMEIDX_OFF          0x00u
#define SYMTABLE_ENTRY_INFO_OFF             0x04u
#define SYMTABLE_ENTRY_OTHER_OFF            0x05u
#define SYMTABLE_ENTRY_SECTIDX_OFF          0x06u
#define SYMTABLE_ENTRY_VALUE_OFF            0x08u
#define SYMTABLE_ENTRY_SIZE_OFF_32BIT       0x0Cu
#define SYMTABLE_ENTRY_SIZE_OFF_64BIT       0x10u


#define SYMTABLE_ENTRY_NAMEIDX_SIZE         4u
#define SYMTABLE_ENTRY_INFO_SIZE            1u
#define SYMTABLE_ENTRY_OTHER_SIZE           1u
#define SYMTABLE_ENTRY_SECTIDX_SIZE         2u
#define SYMTABLE_ENTRY_VALUE_SIZE_32BIT     4u
#define SYMTABLE_ENTRY_VALUE_SIZE_64BIT     8u
#define SYMTABLE_ENTRY_SIZE_SIZE_32BIT      4u
#define SYMTABLE_ENTRY_SIZE_SIZE_64BIT      8u

#define SYMTABLE_STRING_SECT_NAME ".strtab"

#endif /* _IG_ELFPARSER_SYMTABLE_PRIV_H_ */