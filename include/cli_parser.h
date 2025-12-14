/**
 * @file cli_parser.h
 * @brief Command-line argument parsing for advanced password configuration
 * @details Handles parsing of CLI flags like --no-letters, --letters=10, etc.
 *          for the advanced CLI mode of WinPass-Native.
 */

#ifndef CLI_PARSER_H
#define CLI_PARSER_H

#include "common.h"

/**
 * @brief Password configuration structure for advanced generation mode
 * @details Stores per-category enable flags and length settings parsed from
 *          command line arguments. Used by GenerateAdvanced() function.
 */
typedef struct {
    BOOL useLetters;    /**< Enable letters [a-zA-Z] in password */
    BOOL useNumbers;    /**< Enable numbers [0-9] in password */
    BOOL useSymbols;    /**< Enable symbols (!@# etc.) in password */
    int letterLength;   /**< Number of letter characters to generate */
    int numberLength;   /**< Number of numeric characters to generate */
    int symbolLength;   /**< Number of symbol characters to generate */
} PasswordConfig;

/**
 * @brief Parses command line arguments into PasswordConfig structure
 * @param args Array of wide-character argument strings from CommandLineToArgvW()
 * @param count Number of arguments in the array
 * @param config Output structure to populate with parsed configuration
 * @return TRUE if all arguments are valid, FALSE if an invalid flag was found
 * @details Recognizes flags: --no-letters, --no-numbers, --no-symbols,
 *          --letters=N, --numbers=N, --symbols=N (and short forms -l=, -n=, -s=).
 *          Applies default values before processing arguments.
 */
BOOL ParseArguments(LPWSTR* args, int count, PasswordConfig* config);

#endif
