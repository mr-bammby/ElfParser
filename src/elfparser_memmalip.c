#include "../inc_priv/elfparser_memmanip_priv.h"

void* ElfParser_memCpy(void *dest, const void *src, size_t len)
{
	uint8_t         *dest_p = dest;
	const uint8_t	*src_p  = src;
    uint16_t        i       = 0;

	if (!dest && !src)
    {
		return (NULL);
    }
    if (dest == src)
    {
        return (dest);
    }
	while (i < len)
	{
		*dest_p = *src_p;
		dest_p++;
        src_p++;
        i++;
	}

	return (dest);
}

void* ElfParser_memRevCpy(void *dest, const void *src, size_t len)
{
	uint8_t         *dest_p = dest;
	const uint8_t	*src_p  = src;
    uint16_t        i       = 0;

	if (!dest && !src)
    {
		return (NULL);
    }
    if (dest == src)
    {
        return (dest);
    }
    src_p += (len - 1);
	while (i < len)
	{
		*dest_p = *src_p;
		dest_p++;
        src_p--;
        i++;
	}

	return (dest);
}

int16_t ElfParser_memCmp(const void *p1, const void *p2, size_t len)
{
	const uint8_t   *p1_p   = p1;
	const uint8_t	*p2_p   = p2;
    uint16_t        i       = 0;

	if (!p1_p && !p2_p)
    {
		return (-1);
    }
    if (p1_p == p2_p)
    {
        return (0);
    }

	while (i < len)
	{
		if (*p1_p != *p2_p)
        {
            return(((int16_t)*p1_p) -  ((int16_t)*p2_p));
        }
		p1_p++;
        p2_p++;
        i++;
	}
    
	return (0);
}

int16_t ElfParser_strCmp(const char *s1, const char *s2)
{
	if (!s1 && !s2)
    {
		return (-1);
    }
    if (s1 == s2)
    {
        return (0);
    }

	while (*s1 != '\0')
	{
		if (*s1 != *s2)
        {
            return(((int16_t)*s1) -  ((int16_t)*s2));
        }
		s1++;
        s2++;
	}
    
	return (0);
}

int64_t ElfParser_strExtract(const void *map, char **dup, size_t start, size_t len)
{
    char *char_map = (char*)map;
    size_t cnt = start;
    int64_t ret_val = 0;

    if(map == NULL)
    {
        return(-2);
    }
    if (start >= len)
    {
        return (-1);
    }
    while(char_map[cnt] != '\0')
    {
        cnt++;
        if (cnt == len)
        {
            break;
        }
    }
    ret_val = cnt - start + 1;
    *dup = malloc(ret_val);
    if (*dup == NULL)
    {
        return(-3);
    }
    if(ElfParser_memCpy(*dup, &(char_map[start]), ret_val) == NULL)
    {
        return(-4);
    }
    return(ret_val);
}

int64_t ElfParser_strDup(const char *str, char **dup)
{
    size_t cnt = 0;
    int64_t ret_val = 0;

    if(str == NULL)
    {
        return(-2);
    }
    while(str[cnt] != '\0')
    {
        cnt++;
    }
    ret_val = cnt + 1;
    *dup = malloc(ret_val);
    if (*dup == NULL)
    {
        return(-3);
    }
    if(ElfParser_memCpy(*dup, str, ret_val) == NULL)
    {
        return(-4);
    }
    return(ret_val);
}
