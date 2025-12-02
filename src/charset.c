/**
 * @file charset.c
 * @brief Character set constant definitions for password generation
 * @details Defines the available character pools used for cryptographic random
 *          password generation. Each charset is carefully selected to balance
 *          security and usability.
 */

#include "../include/common.h"

/* Full charset: 84 characters (52 letters + 10 digits + 22 symbols) for maximum entropy */
const char CHARSET_FULL[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()-_=+[]{}<?>";

/* Alphanumeric: 62 characters, suitable for systems that reject special characters */
const char CHARSET_ALPHANUM[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

/* Letters only: 52 characters (26 lowercase + 26 uppercase) */
const char CHARSET_LETTERS[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

/* Digits only: 10 characters, useful for PIN-like passwords */
const char CHARSET_NUMBERS[] = "0123456789";

/* Symbols: 22 common special characters, avoiding problematic ones like ` or quotes */
const char CHARSET_SYMBOLS[] = "!@#$%^&*()-_=+[]{}<?>";
