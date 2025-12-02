/**
 * @file cli_parser.c
 * @brief CLI argument parsing implementation for advanced password configuration
 * @details Parses command-line flags and values to configure password generation
 *          with per-category control over character types and lengths.
 */

#include "../include/cli_parser.h"
#include "../include/utils.h"

/**
 * @brief Parses command line arguments into PasswordConfig structure
 * @param args Array of wide-character argument strings
 * @param count Number of arguments
 * @param config Output configuration structure
 */
void ParseArguments(LPWSTR* args, int count, PasswordConfig* config) {
    /* Initialize with sensible defaults (all categories enabled, moderate lengths) */
    config->useLetters = TRUE;
    config->useNumbers = TRUE;
    config->useSymbols = TRUE;
    config->letterLength = 8;
    config->numberLength = 4;
    config->symbolLength = 4;

    /* Process all arguments starting from index 1 (skip program name at index 0) */
    for (int i = 1; i < count; i++) {
        LPWSTR arg = args[i];
        
        /* Disable flags: turn off specific character categories */
        if (WStrEquals(arg, "--no-letters")) {
            config->useLetters = FALSE;
        }
        else if (WStrEquals(arg, "--no-numbers")) {
            config->useNumbers = FALSE;
        }
        else if (WStrEquals(arg, "--no-symbols")) {
            config->useSymbols = FALSE;
        }
        /* Length configuration: parse value after '=' delimiter */
        else if (WStrStartsWith(arg, "--letters=") || WStrStartsWith(arg, "-l=")) {
            int val = ExtractValueFromArg(arg);
            /* Bounds check to prevent integer overflow or unreasonable values */
            if (val >= 0 && val < 1024) config->letterLength = val;
        }
        else if (WStrStartsWith(arg, "--numbers=") || WStrStartsWith(arg, "-n=")) {
            int val = ExtractValueFromArg(arg);
            if (val >= 0 && val < 1024) config->numberLength = val;
        }
        else if (WStrStartsWith(arg, "--symbols=") || WStrStartsWith(arg, "-s=")) {
            int val = ExtractValueFromArg(arg);
            if (val >= 0 && val < 1024) config->symbolLength = val;
        }
        /* Unrecognized arguments are silently ignored for forward compatibility */
    }
}