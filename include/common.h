/**
 * @file common.h
 * @brief Common includes, constants, and charset declarations for WinPass-Native
 * @details This header provides platform-specific Windows API includes and declares
 *          character sets used for password generation. The project uses pure Win32 API
 *          without relying on standard C library (stdio.h/stdlib.h).
 * 
 * Project: WinPass-Native
 * Version: 2.0.0 (Advanced Configuration)
 */

#ifndef COMMON_H
#define COMMON_H

#define _WIN32_WINNT 0x0500  /**< Target Windows 2000+ for console API support */
#include <windows.h>         /**< Core Win32 API */
#include <wincrypt.h>        /**< Cryptographic API for secure random generation */
#include <shellapi.h>        /**< Command line parsing and clipboard operations */

/* Password length constraints */
#define MIN_PASSWORD_LENGTH  4      /**< Minimum total password length for security */
#define MAX_PASSWORD_LENGTH  1024   /**< Maximum total password length */
#define MAX_CATEGORY_LENGTH  1024   /**< Maximum length per character category */
#define DEFAULT_BATCH_LENGTH 16     /**< Default password length for batch mode */
#define MAX_INT_PARSE_VALUE  100000 /**< Maximum value for integer parsing to prevent overflow */

/**
 * @brief Full character set including letters, numbers, and symbols
 * @details Contains 84 characters: [a-z][A-Z][0-9] + common symbols
 */
extern const char CHARSET_FULL[];

/**
 * @brief Alphanumeric character set (letters and numbers only)
 * @details Contains 62 characters: [a-z][A-Z][0-9]
 */
extern const char CHARSET_ALPHANUM[];

/**
 * @brief Letters only character set (uppercase and lowercase)
 * @details Contains 52 characters: [a-z][A-Z]
 */
extern const char CHARSET_LETTERS[];

/**
 * @brief Numeric digits character set
 * @details Contains 10 characters: [0-9]
 */
extern const char CHARSET_NUMBERS[];

/**
 * @brief Symbol characters for password complexity
 * @details Contains 22 special characters: !@#$%^&*()-_=+[]{}<?> etc.
 */
extern const char CHARSET_SYMBOLS[];

#endif