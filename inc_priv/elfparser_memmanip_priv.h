#ifndef _IG_ELFPARSER_MEMMANIP_PRIV_H_
#define _IG_ELFPARSER_MEMMANIP_PRIV_H_

#include <inttypes.h>
#include <stdlib.h>

void* ElfParser_memCpy(void *dest, const void *src, size_t len);
void* ElfParser_memRevCpy(void *dest, const void *src, size_t len);
int16_t ElfParser_memCmp(const void *p1, const void *p2, size_t len);
int64_t ElfParser_strExtract(const void *map, char **dup, size_t start, size_t len);
int64_t ElfParser_strDup(const char *str, char **dup);


#endif /* _IG_ELFPARSER_MEMMANIP_PRIV_H_ */