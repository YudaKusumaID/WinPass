/**
 * @file main.c
 * @brief WinPass-Native - Secure Password Generator
 * @details Main entry point for password generator with three operation modes:
 *          1. Legacy Batch Mode: Single numeric argument (e.g., WinPass.exe 20)
 *          2. Advanced CLI Mode: Flags and options (e.g., --letters=10 --no-symbols)
 *          3. Interactive Mode: No arguments, displays menu interface
 * 
 * Project: WinPass-Native
 * Version: 2.0.0 (Advanced Configuration)
 * 
 * Compliance:
 * - No <stdio.h>/<stdlib.h> (Pure Win32 API)
 * - Cryptographically secure random generation (CryptoAPI)
 * - ReadConsole for input (no scanf)
 * - Toggle per-category (Letters, Numbers, Symbols)
 * - Configurable length per-category
 */

#include "include/common.h"
#include "include/console_io.h"
#include "include/password_gen.h"
#include "include/cli_parser.h"
#include "include/interactive.h"
#include "include/utils.h"

/**
 * @brief Main entry point - detects operation mode and routes execution
 * @return 0 on successful execution
 * @details Parses command line to determine which of three modes to use:
 *          - Legacy batch: single numeric arg without flags
 *          - Advanced CLI: arguments with flags/options
 *          - Interactive: no arguments provided
 */
int main() {
    LPWSTR *szArglist;
    int nArgs;

    /* Parse Windows command line into argument array */
    szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);

    /* Check for help flag before mode detection */
    if (NULL != szArglist && nArgs > 1) {
        /* Scan all arguments for help flags */
        for (int i = 1; i < nArgs; i++) {
            if (WStrEquals(szArglist[i], "--help") || 
                WStrEquals(szArglist[i], "-h") ||
                WStrEquals(szArglist[i], "/?") ||
                WStrEquals(szArglist[i], "-?")) {
                ShowHelp();
                if (szArglist) LocalFree(szArglist);
                return 0;
            }
        }
    }

    if (NULL != szArglist && nArgs > 1) {
        /*
         * Arguments detected - determine if legacy batch or advanced CLI mode
         * Legacy batch: single argument without '-' prefix (e.g., "20")
         * Advanced CLI: arguments with flags like "--letters=10"
         */
        BOOL isLegacyBatch = FALSE;
        if (nArgs == 2) {
            /* Check if argument doesn't start with '-' flag prefix */
            if (szArglist[1][0] != L'-') {
                isLegacyBatch = TRUE;
            }
        }

        if (isLegacyBatch) {
            /* MODE 1: LEGACY BATCH MODE - single numeric arg without flags */
            int batchLength = SimpleWStrToInt(szArglist[1]);
            if (batchLength <= 0) batchLength = DEFAULT_BATCH_LENGTH;  /* Fallback to safe default */

            ConsoleWrite("WinPass-Native (Batch Mode)\r\n");
            GenerateCore(batchLength, TRUE); /* Default symbols enabled for batch */
        }
        else {
            /* MODE 2: ADVANCED CLI MODE - parse flags and generate */
            PasswordConfig config;
            if (!ParseArguments(szArglist, nArgs, &config)) {
                if (szArglist) LocalFree(szArglist);
                return 1;
            }

            ConsoleWrite("WinPass-Native (Advanced CLI Mode)\r\n");
            GenerateAdvanced(config.letterLength, config.numberLength, config.symbolLength,
                           config.useLetters, config.useNumbers, config.useSymbols);
        }
    }
    else {
        /* MODE 3: INTERACTIVE MODE - no arguments, show menu interface */
        RunInteractiveMode();
    }

    /* Free command line argument memory allocated by CommandLineToArgvW */
    if (szArglist) LocalFree(szArglist);
    return 0;
}