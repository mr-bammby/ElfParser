/**
 * @file elfparser_common.h
 * @brief Public header for common definitions in libelfparser
 * @author Domen Banfi
 * @date 2025-03-16
 * @version 1.0
 *
 * This header defines common constants and types used across the standalone
 * libelfparser library, specifically the error codes returned by library
 * functions. It serves as a central point for return value definitions,
 * ensuring consistency in error handling across all public interfaces.
 */

#ifndef _IG_ELFPARSER_COMMON_H_
#define _IG_ELFPARSER_COMMON_H_

/**
 * @brief Enumeration of error codes returned by libelfparser functions
 * @enum ElfParser_Error
 */
enum ElfParser_Error {
    ELFPARSER_SUCCESS       = 0,    /**< Operation completed successfully */
    ELFPARSER_ERR_SIZE      = -1,   /**< Insufficient size provided (e.g., buffer or map too small) */
    ELFPARSER_ERR_NULL      = -2,   /**< Null pointer input provided */
    ELFPARSER_ERR_FORMAT    = -3,   /**< Invalid ELF format (e.g., incorrect magic number) */
    ELFPARSER_ERR_CLASS     = -4,   /**< Invalid ELF class or endianness */
    ELFPARSER_ERR_MALLOC    = -5,   /**< Memory allocation failed */
    ELFPARSER_ERR_RANGE     = -6,   /**< Range or bounds error (e.g., invalid index or overflow) */
    ELFPARSER_ERR_NOT_FOUND = -7,   /**< Requested item (e.g., section, symbol) not found */
    ELFPARSER_ERR_MEMCPY    = -8    /**< Memory copy operation failed */
};

#endif /* _IG_ELFPARSER_COMMON_H_ */
