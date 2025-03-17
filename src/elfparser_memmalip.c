/**
 * @file elfparser_memmanip_priv.c
 * @brief Private memory manipulation utilities for libelfparser
 * @author Domen Banfi
 * @date 2025-03-16
 * @version 1.0
 *
 * This file implements private memory manipulation functions for the standalone
 * libelfparser library. These utilities handle copying, comparing, and duplicating
 * memory and strings, intended for internal use by other library components.
 */

#include "../inc_priv/elfparser_memmanip_priv.h"
#include "../inc_pub/elfparser_common.h"

/**
 * @brief Copies a block of memory from source to destination
 * @param[out] dest Pointer to the destination memory
 * @param[in] src Pointer to the source memory
 * @param[in] len Number of bytes to copy
 * @return void* Pointer to dest on success, NULL on failure (NULL input or overflow)
 */
void* ElfParser_memCpy(void *dest, const void *src, size_t len)
{
    uint8_t         *dest_p = dest;
    const uint8_t   *src_p  = src;
    size_t          i       = 0;  // Use size_t for larger ranges

    if (!dest || !src)  // Check either pointer being NULL
    {
        return NULL;
    }
    if (dest == src)
    {
        return dest;
    }
    if (len > SIZE_MAX)  // Basic overflow check
    {
        return NULL;
    }
    while (i < len)
    {
        *dest_p++ = *src_p++;
        i++;
    }
    return dest;
}

/**
 * @brief Copies a block of memory from source to destination in reverse order
 * @param[out] dest Pointer to the destination memory
 * @param[in] src Pointer to the source memory
 * @param[in] len Number of bytes to copy
 * @return void* Pointer to dest on success, NULL on failure (NULL input or overflow)
 */
void* ElfParser_memRevCpy(void *dest, const void *src, size_t len)
{
    uint8_t         *dest_p = dest;
    const uint8_t   *src_p  = src;
    size_t          i       = 0;

    if (!dest || !src)
    {
        return NULL;
    }
    if (dest == src)
    {
        return dest;
    }
    if (len > SIZE_MAX)
    {
        return NULL;
    }
    src_p += (len - 1);
    while (i < len)
    {
        *dest_p++ = *src_p--;
        i++;
    }
    return dest;
}

/**
 * @brief Compares two blocks of memory
 * @param[in] p1 Pointer to the first memory block
 * @param[in] p2 Pointer to the second memory block
 * @param[in] len Number of bytes to compare
 * @return int16_t 0 if equal, difference between first differing bytes, ELFPARSER_ERR_NULL if both NULL
 */
int16_t ElfParser_memCmp(const void *p1, const void *p2, size_t len)
{
    const uint8_t   *p1_p   = p1;
    const uint8_t   *p2_p   = p2;
    size_t          i       = 0;

    if (!p1_p || !p2_p)  // Check either pointer being NULL
    {
        return ELFPARSER_ERR_NULL;
    }
    if (p1_p == p2_p)
    {
        return 0;
    }
    if (len > SIZE_MAX)
    {
        return ELFPARSER_ERR_RANGE;
    }
    while (i < len)
    {
        if (*p1_p != *p2_p)
        {
            return ((int16_t)*p1_p) - ((int16_t)*p2_p);
        }
        p1_p++;
        p2_p++;
        i++;
    }
    return 0;
}

/**
 * @brief Compares two null-terminated strings
 * @param[in] s1 First string
 * @param[in] s2 Second string
 * @return int16_t 0 if equal, difference between first differing chars, ELFPARSER_ERR_NULL if both NULL
 */
int16_t ElfParser_strCmp(const char *s1, const char *s2)
{
    if (!s1 || !s2)  // Check either string being NULL
    {
        return ELFPARSER_ERR_NULL;
    }
    if (s1 == s2)
    {
        return 0;
    }
    while (*s1 != '\0')
    {
        if (*s1 != *s2)
        {
            return ((int16_t)*s1) - ((int16_t)*s2);
        }
        s1++;
        s2++;
    }
    return ((int16_t)*s1) - ((int16_t)*s2);
}

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
int64_t ElfParser_strExtract(const void *map, char **dup, size_t start, size_t len)
{
    char *char_map = (char*)map;
    size_t cnt = start;
    int64_t ret_val = 0;

    if (!map || !dup)
    {
        return ELFPARSER_ERR_NULL;
    }
    if (start >= len)
    {
        return ELFPARSER_ERR_RANGE;
    }
    while (char_map[cnt] != '\0' && cnt < len)
    {
        cnt++;
    }
    ret_val = cnt - start + 1;  // Include null terminator
    if (ret_val <= 0 || ret_val > INTMAX_MAX)
    {
        return ELFPARSER_ERR_RANGE;
    }
    *dup = malloc(ret_val);
    if (!*dup)
    {
        return ELFPARSER_ERR_MALLOC;
    }
    if (!ElfParser_memCpy(*dup, &(char_map[start]), ret_val))
    {
        free(*dup);  // Cleanup on failure
        *dup = NULL;
        return ELFPARSER_ERR_MEMCPY;  // Define in elfparser_common.h
    }
    return ret_val;
}

/**
 * @brief Duplicates a null-terminated string
 * @param[in] str Source string to duplicate
 * @param[out] dup Pointer to store the duplicated string
 * @return int64_t Length of duplicated string on success, ELFPARSER_ERR_NULL if str is NULL,
 *                 ELFPARSER_ERR_MALLOC if malloc fails, ELFPARSER_ERR_MEMCPY if copy fails
 */
int64_t ElfParser_strDup(const char *str, char **dup)
{
    size_t cnt = 0;
    int64_t ret_val = 0;

    if (!str || !dup)
    {
        return ELFPARSER_ERR_NULL;
    }
    while (str[cnt] != '\0')
    {
        cnt++;
        if (cnt == SIZE_MAX)  // Prevent overflow
        {
            return ELFPARSER_ERR_RANGE;
        }
    }
    ret_val = cnt + 1;  // Include null terminator
    *dup = malloc(ret_val);
    if (!*dup)
    {
        return ELFPARSER_ERR_MALLOC;
    }
    if (!ElfParser_memCpy(*dup, str, ret_val))
    {
        free(*dup);  // Cleanup on failure
        *dup = NULL;
        return ELFPARSER_ERR_MEMCPY;
    }
    return ret_val;
}
