/**
 * @file elfparser_memmanip_priv.h
 * @brief Private header for memory manipulation utilities in libelfparser
 * @author Domen Banfi
 * @date 2025-03-16
 * @version 1.0
 *
 * This header declares private memory manipulation functions for the standalone
 * libelfparser library. These utilities provide low-level operations for copying,
 * comparing, and duplicating memory and strings, intended for internal use by
 * other library components (e.g., elfparser_secthead.c, elfparser_symtable.c).
 * They are not part of the public API.
 */

#ifndef _IG_ELFPARSER_MEMMANIP_PRIV_H_
#define _IG_ELFPARSER_MEMMANIP_PRIV_H_

#include <inttypes.h>
#include <stdlib.h>

/**
 * @brief Copies a block of memory from source to destination
 * @param[out] dest Pointer to the destination memory
 * @param[in] src Pointer to the source memory
 * @param[in] len Number of bytes to copy
 * @return void* Pointer to dest on success, NULL on failure (NULL input or overflow)
 */
void* ElfParser_memCpy(void *dest, const void *src, size_t len);

/**
 * @brief Copies a block of memory from source to destination in reverse order
 * @param[out] dest Pointer to the destination memory
 * @param[in] src Pointer to the source memory
 * @param[in] len Number of bytes to copy
 * @return void* Pointer to dest on success, NULL on failure (NULL input or overflow)
 */
void* ElfParser_memRevCpy(void *dest, const void *src, size_t len);

/**
 * @brief Compares two null-terminated strings
 * @param[in] s1 First string
 * @param[in] s2 Second string
 * @return int16_t 0 if equal, difference between first differing chars, ELFPARSER_ERR_NULL if both NULL
 */
int16_t ElfParser_strCmp(const char *s1, const char *s2);

/**
 * @brief Compares two blocks of memory
 * @param[in] p1 Pointer to the first memory block
 * @param[in] p2 Pointer to the second memory block
 * @param[in] len Number of bytes to compare
 * @return int16_t 0 if equal, difference between first differing bytes, ELFPARSER_ERR_NULL if both NULL
 */
int16_t ElfParser_memCmp(const void *p1, const void *p2, size_t len);

/**
 * @brief Extracts and duplicates a substring from a memory map
 * @param[in] map Pointer to the memory-mapped file
 * @param[out] dup Pointer to store the duplicated substring
 * @param[in] start Starting index in map
 * @param[in] len Maximum length of map
 * @return int64_t Length of duplicated string on success, ELFPARSER_ERR_NULL if map is NULL,
 *                 ELFPARSER_ERR_RANGE if start >= len, ELFPARSER_ERR_MALLOC if malloc fails,
 *                 ELFPARSER_ERR_MEMCPY if copy fails
 */
int64_t ElfParser_strExtract(const void *map, char **dup, size_t start, size_t len);

/**
 * @brief Duplicates a null-terminated string
 * @param[in] str Source string to duplicate
 * @param[out] dup Pointer to store the duplicated string
 * @return int64_t Length of duplicated string on success, ELFPARSER_ERR_NULL if str is NULL,
 *                 ELFPARSER_ERR_MALLOC if malloc fails, ELFPARSER_ERR_MEMCPY if copy fails
 */
int64_t ElfParser_strDup(const char *str, char **dup);

#endif /* _IG_ELFPARSER_MEMMANIP_PRIV_H_ */