/**
 * @file cli_parser.c
 * @brief CLI argument parsing implementation for advanced password configuration
 * @details Parses command-line flags and values to configure password generation
 *          with per-category control over character types and lengths.
 */

#include "../include/cli_parser.h"
#include "../include/utils.h"
#include "../include/console_io.h"

/**
 * @brief Parses command line arguments into PasswordConfig structure
 * @param args Array of wide-character argument strings
 * @param count Number of arguments
 * @param config Output configuration structure
 * @return TRUE if all arguments are valid, FALSE if an invalid flag was found
 */
BOOL ParseArguments(LPWSTR* args, int count, PasswordConfig* config) {
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
        BOOL recognized = FALSE;
        
        /* Disable flags: turn off specific character categories */
        if (WStrEquals(arg, "--no-letters")) {
            config->useLetters = FALSE;
            recognized = TRUE;
        }
        else if (WStrEquals(arg, "--no-numbers")) {
            config->useNumbers = FALSE;
            recognized = TRUE;
        }
        else if (WStrEquals(arg, "--no-symbols")) {
            config->useSymbols = FALSE;
            recognized = TRUE;
        }
        /* Length configuration: parse value after '=' delimiter */
        else if (WStrStartsWith(arg, "--letters=") || WStrStartsWith(arg, "-l=")) {
            int val = ExtractValueFromArg(arg);
            if (val == -2) {
                ConsoleWrite("[ERROR] Invalid value for --letters. Expected a number.\r\n");
                return FALSE;
            }
            if (val >= 0 && val < MAX_CATEGORY_LENGTH) config->letterLength = val;
            recognized = TRUE;
        }
        else if (WStrStartsWith(arg, "--numbers=") || WStrStartsWith(arg, "-n=")) {
            int val = ExtractValueFromArg(arg);
            if (val == -2) {
                ConsoleWrite("[ERROR] Invalid value for --numbers. Expected a number.\r\n");
                return FALSE;
            }
            if (val >= 0 && val < MAX_CATEGORY_LENGTH) config->numberLength = val;
            recognized = TRUE;
        }
        else if (WStrStartsWith(arg, "--symbols=") || WStrStartsWith(arg, "-s=")) {
            int val = ExtractValueFromArg(arg);
            if (val == -2) {
                ConsoleWrite("[ERROR] Invalid value for --symbols. Expected a number.\r\n");
                return FALSE;
            }
            if (val >= 0 && val < MAX_CATEGORY_LENGTH) config->symbolLength = val;
            recognized = TRUE;
        }
        
        /* Check for unrecognized flag (starts with '-') */
        if (!recognized && arg[0] == L'-') {
            char errorBuf[256];
            char narrowArg[64];
            
            /* Convert wide string to narrow for error message */
            int j = 0;
            while (arg[j] != L'\0' && j < 63) {
                narrowArg[j] = (char)arg[j];
                j++;
            }
            narrowArg[j] = '\0';
            
            wsprintfA(errorBuf, "[ERROR] Unknown flag: %s\r\n", narrowArg);
            ConsoleWrite(errorBuf);
            ConsoleWrite("Use --help to see available options.\r\n");
            return FALSE;
        }
    }
    
    return TRUE;
}