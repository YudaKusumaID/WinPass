/**
 * @file utils.h
 * @brief Utility helper functions for string manipulation and parsing
 * @details Provides custom string conversion and comparison functions that work with
 *          both narrow (char) and wide (WCHAR) characters, avoiding standard library
 *          dependencies for Win32 API compatibility.
 */

#ifndef UTILS_H
#define UTILS_H

#include "common.h"

/**
 * @brief Converts ASCII string to integer
 * @param str Null-terminated ASCII string containing numeric digits
 * @return Parsed integer value, 0 if no valid digits found
 * @details Simple digit-by-digit conversion without error checking for non-digits
 */
int SimpleStrToInt(const char* str);

/**
 * @brief Converts wide character string to integer
 * @param str Null-terminated wide character string containing numeric digits
 * @return Parsed integer value, 0 if no valid digits found
 * @details Wide character version for processing command line arguments (LPWSTR)
 */
int SimpleWStrToInt(const WCHAR* str);

/**
 * @brief Compares wide string with ASCII string for equality
 * @param wstr Wide character string (from command line arguments)
 * @param str ASCII string (literal comparison target)
 * @return TRUE if strings match exactly, FALSE otherwise
 * @details Used for CLI flag matching like "--no-letters"
 */
BOOL WStrEquals(const WCHAR* wstr, const char* str);

/**
 * @brief Checks if wide string starts with ASCII prefix
 * @param wstr Wide character string to check
 * @param prefix ASCII prefix to match
 * @return TRUE if wstr starts with prefix, FALSE otherwise
 * @details Used for parsing arguments with values like "--letters=10"
 */
BOOL WStrStartsWith(const WCHAR* wstr, const char* prefix);

/**
 * @brief Extracts numeric value from command line argument
 * @param arg Wide character argument like "--letters=10" or "-l=5"
 * @return Parsed integer value after '=' delimiter, -1 if no '=' found
 * @details Searches for '=' separator and parses the numeric value after it
 */
int ExtractValueFromArg(const WCHAR* arg);

/**
 * @brief Checks if wide string contains only numeric digits
 * @param wstr Wide character string to validate
 * @return TRUE if string contains only digits (0-9), FALSE otherwise
 */
BOOL IsWStrNumeric(const WCHAR* wstr);

#endif
