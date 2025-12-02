/**
 * @file password_gen.h
 * @brief Password generation core logic with cryptographic randomness
 * @details Provides password generation functions using Windows CryptoAPI for
 *          secure random number generation. Supports both simple (core) and
 *          advanced (multi-charset with configurable lengths) generation modes.
 */

#ifndef PASSWORD_GEN_H
#define PASSWORD_GEN_H

#include "common.h"

/**
 * @brief Copies generated password to Windows clipboard
 * @param text Password string to copy
 * @param length Length of password (without null terminator)
 * @details Uses Win32 clipboard API (GlobalAlloc, SetClipboardData) to enable
 *          easy password pasting after generation
 */
void CopyToClipboard(const char* text, int length);

/**
 * @brief Shuffles password characters using Fisher-Yates algorithm
 * @param password Password string to shuffle in-place
 * @param length Length of password
 * @param hCryptProv Cryptographic context for secure random bytes
 * @details Uses cryptographically secure random numbers to ensure uniform distribution
 */
void ShufflePassword(char* password, int length, HCRYPTPROV hCryptProv);

/**
 * @brief Generates password with simple configuration (legacy/batch mode)
 * @param length Total password length
 * @param useSymbols TRUE to include symbols, FALSE for alphanumeric only
 * @details Uses either CHARSET_FULL or CHARSET_ALPHANUM depending on useSymbols flag.
 *          Automatically copies result to clipboard.
 */
void GenerateCore(int length, BOOL useSymbols);

/**
 * @brief Generates password with advanced per-category configuration
 * @param letterCount Number of letter characters [a-zA-Z]
 * @param numberCount Number of numeric characters [0-9]
 * @param symbolCount Number of symbol characters
 * @param useLetters TRUE to enable letters category
 * @param useNumbers TRUE to enable numbers category
 * @param useSymbols TRUE to enable symbols category
 * @details Assembles password from separate character categories, then shuffles
 *          using Fisher-Yates algorithm for uniform distribution. Validates that
 *          at least one category is enabled.
 */
void GenerateAdvanced(int letterCount, int numberCount, int symbolCount,
                      BOOL useLetters, BOOL useNumbers, BOOL useSymbols);

#endif
