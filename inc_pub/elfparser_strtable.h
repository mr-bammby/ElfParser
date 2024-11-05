#ifndef _IG_ELFPARSER_STRTABLE_H_
#define _IG_ELFPARSER_STRTABLE_H_
#include <stdlib.h>

typedef char ** elfparser_strtable_t;

int ElfParser_StrTable_parse(elfparser_strtable_t *string_table, const void *map, size_t map_size);
int ElfParser_StrTable_free(elfparser_strtable_t *string_table);


#endif /* _IG_ELFPARSER_STRTABLE_H_ */