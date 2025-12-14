/**
 * @file utils.c
 * @brief Utility helper functions implementation
 * @details Custom string manipulation functions that avoid standard library
 *          dependencies (no atoi, strcmp, etc.) for pure Win32 implementation.
 */

#include "../include/utils.h"

/**
 * @brief Converts ASCII string to integer with overflow protection
 * @param str Null-terminated ASCII string
 * @return Parsed integer value, capped at MAX_INT_PARSE_VALUE
 */
int SimpleStrToInt(const char* str) {
    int res = 0;
    /* Accumulate digits using base-10 positional notation */
    while ((*str >= '0') && (*str <= '9')) {
        /* Check for potential overflow before multiplication */
        if (res > MAX_INT_PARSE_VALUE / 10) {
            return MAX_INT_PARSE_VALUE;  /* Cap at maximum safe value */
        }
        res = (res * 10) + (*str - '0');
        if (res > MAX_INT_PARSE_VALUE) {
            return MAX_INT_PARSE_VALUE;  /* Cap if exceeded after addition */
        }
        str++;
    }
    return res;
}

/**
 * @brief Converts wide character string to integer with overflow protection
 * @param str Null-terminated wide character string
 * @return Parsed integer value, capped at MAX_INT_PARSE_VALUE
 */
int SimpleWStrToInt(const WCHAR* str) {
    int res = 0;
    /* Same algorithm as SimpleStrToInt but for WCHAR (L'0' instead of '0') */
    while ((*str >= L'0') && (*str <= L'9')) {
        /* Check for potential overflow before multiplication */
        if (res > MAX_INT_PARSE_VALUE / 10) {
            return MAX_INT_PARSE_VALUE;  /* Cap at maximum safe value */
        }
        res = (res * 10) + (*str - L'0');
        if (res > MAX_INT_PARSE_VALUE) {
            return MAX_INT_PARSE_VALUE;  /* Cap if exceeded after addition */
        }
        str++;
    }
    return res;
}

/**
 * @brief Compares wide string with ASCII string for equality
 * @param wstr Wide character string (from CommandLineToArgvW)
 * @param str ASCII string literal for comparison
 * @return TRUE if equal, FALSE otherwise
 */
BOOL WStrEquals(const WCHAR* wstr, const char* str) {
    /* Character-by-character comparison with type casting */
    while (*str != '\0') {
        if (*wstr != (WCHAR)(*str)) return FALSE;  /* Mismatch found */
        wstr++;
        str++;
    }
    return (*wstr == L'\0');  /* Ensure wstr also ends here (same length check) */
}

/**
 * @brief Checks if wide string starts with ASCII prefix
 * @param wstr Wide character string to check
 * @param prefix ASCII prefix to match
 * @return TRUE if starts with prefix, FALSE otherwise
 */
BOOL WStrStartsWith(const WCHAR* wstr, const char* prefix) {
    /* Similar to WStrEquals but doesn't require full string match */
    while (*prefix != '\0') {
        if (*wstr != (WCHAR)(*prefix)) return FALSE;
        wstr++;
        prefix++;
    }
    return TRUE;  /* Prefix matched, don't care about remaining wstr characters */
}

/**
 * @brief Extracts numeric value from key=value argument format
 * @param arg Wide character argument like "--letters=10"
 * @return Parsed integer after '=', or -1 if no '=' found
 */
int ExtractValueFromArg(const WCHAR* arg) {
    /* Scan forward until we find '=' delimiter or reach end */
    while (*arg != L'\0' && *arg != L'=') {
        arg++;
    }
    if (*arg == L'=') {
        arg++;  /* Skip past the '=' to point at value string */
        return SimpleWStrToInt(arg);
    }
    return -1;  /* No '=' found, invalid format */
}