/**
 * @file interactive.c
 * @brief Interactive menu mode implementation for password generation
 * @details Provides a console-based menu interface with real-time configuration
 *          display and password generation. Loops until user chooses to exit.
 */

#include "../include/interactive.h"
#include "../include/console_io.h"
#include "../include/password_gen.h"
#include "../include/utils.h"

/**
 * @brief Validates if a string contains only numeric digits
 * @param str Null-terminated string to validate
 * @return TRUE if string contains only digits, FALSE otherwise
 */
static BOOL IsNumericInput(const char* str) {
    /* Empty string is not valid numeric input */
    if (*str == '\0') return FALSE;
    
    while (*str != '\0') {
        if (*str < '0' || *str > '9') return FALSE;
        str++;
    }
    return TRUE;
}

/**
 * @brief Runs the interactive menu mode with full configuration options
 * @details Main loop that displays menu, handles user input, and updates settings
 */
void RunInteractiveMode() {
    BOOL running = TRUE;
    
    /* Default configuration: all categories enabled with moderate lengths */
    BOOL useLetters = TRUE;
    BOOL useNumbers = TRUE;
    BOOL useSymbols = TRUE;
    int letterLength = 8;
    int numberLength = 4;
    int symbolLength = 4;
    
    char inputBuf[32];
    char displayBuf[256];

    while (running) {
        ClearScreen();
        
        ConsoleWrite("=== WinPass Interactive Mode (Advanced) ===\r\n");
        
        /* Calculate and display current total password length from enabled categories */
        int totalLen = 0;
        if (useLetters) totalLen += letterLength;
        if (useNumbers) totalLen += numberLength;
        if (useSymbols) totalLen += symbolLength;

        wsprintfA(displayBuf, "\r\n[Settings] Total: %d chars\r\n", totalLen);
        ConsoleWrite(displayBuf);
        
        /* Show current configuration with ON/OFF status and length for each category */
        wsprintfA(displayBuf, "  Letters: %s (%d) | Numbers: %s (%d) | Symbols: %s (%d)\r\n",
                  useLetters ? "ON " : "OFF",
                  letterLength,
                  useNumbers ? "ON " : "OFF",
                  numberLength,
                  useSymbols ? "ON " : "OFF",
                  symbolLength);
        ConsoleWrite(displayBuf);

        /* Display menu options */
        ConsoleWrite("\r\n1. Generate Password\r\n");
        ConsoleWrite("2. Toggle Letters\r\n");
        ConsoleWrite("3. Toggle Numbers\r\n");
        ConsoleWrite("4. Toggle Symbols\r\n");
        ConsoleWrite("5. Set Letter Length\r\n");
        ConsoleWrite("6. Set Number Length\r\n");
        ConsoleWrite("7. Set Symbol Length\r\n");
        ConsoleWrite("8. Exit\r\n");
        ConsoleWrite("Select > ");

        /* Read and process user choice */
        int readLen = ConsoleRead(inputBuf, sizeof(inputBuf));
        if (readLen > 0) {
            /* Convert input to integer and validate range 1-8 */
            int choice = SimpleStrToInt(inputBuf);
            
            if (choice < 1 || choice > 8) {
                /* Invalid choice: outside valid range or non-numeric */
                ConsoleWrite("\r\n[ERROR] Invalid option! Please select 1-8.\r\n");
                ConsoleWrite("Press Enter to continue...");
                ConsoleRead(inputBuf, sizeof(inputBuf));
                continue;
            }

            switch (choice) {
                case 1:
                    /* Generate password with current configuration */
                    GenerateAdvanced(letterLength, numberLength, symbolLength,
                                   useLetters, useNumbers, useSymbols);
                    break;
                    
                /* Toggle options: flip boolean state */
                case 2:
                    useLetters = !useLetters;
                    break;
                case 3:
                    useNumbers = !useNumbers;
                    break;
                case 4:
                    useSymbols = !useSymbols;
                    break;
                    
                /* Length configuration options: prompt for new value with validation */
                case 5: {
                    ClearScreen();
                    wsprintfA(displayBuf, "Enter letter length (0-%d): ", MAX_CATEGORY_LENGTH - 1);
                    ConsoleWrite(displayBuf);
                    if (ConsoleRead(inputBuf, sizeof(inputBuf)) > 0) {
                        if (!IsNumericInput(inputBuf)) {
                            ConsoleWrite("[ERROR] Please enter a valid number!\r\n");
                            ConsoleWrite("Press Enter to continue...");
                            ConsoleRead(inputBuf, sizeof(inputBuf));
                        } else {
                            int newLen = SimpleStrToInt(inputBuf);
                            if (newLen >= 0 && newLen < MAX_CATEGORY_LENGTH) {
                                letterLength = newLen;
                            } else {
                                wsprintfA(displayBuf, "[ERROR] Length must be between 0 and %d!\r\n", MAX_CATEGORY_LENGTH - 1);
                                ConsoleWrite(displayBuf);
                                ConsoleWrite("Press Enter to continue...");
                                ConsoleRead(inputBuf, sizeof(inputBuf));
                            }
                        }
                    }
                    break;
                }
                case 6: {
                    ClearScreen();
                    wsprintfA(displayBuf, "Enter number length (0-%d): ", MAX_CATEGORY_LENGTH - 1);
                    ConsoleWrite(displayBuf);
                    if (ConsoleRead(inputBuf, sizeof(inputBuf)) > 0) {
                        if (!IsNumericInput(inputBuf)) {
                            ConsoleWrite("[ERROR] Please enter a valid number!\r\n");
                            ConsoleWrite("Press Enter to continue...");
                            ConsoleRead(inputBuf, sizeof(inputBuf));
                        } else {
                            int newLen = SimpleStrToInt(inputBuf);
                            if (newLen >= 0 && newLen < MAX_CATEGORY_LENGTH) {
                                numberLength = newLen;
                            } else {
                                wsprintfA(displayBuf, "[ERROR] Length must be between 0 and %d!\r\n", MAX_CATEGORY_LENGTH - 1);
                                ConsoleWrite(displayBuf);
                                ConsoleWrite("Press Enter to continue...");
                                ConsoleRead(inputBuf, sizeof(inputBuf));
                            }
                        }
                    }
                    break;
                }
                case 7: {
                    ClearScreen();
                    wsprintfA(displayBuf, "Enter symbol length (0-%d): ", MAX_CATEGORY_LENGTH - 1);
                    ConsoleWrite(displayBuf);
                    if (ConsoleRead(inputBuf, sizeof(inputBuf)) > 0) {
                        if (!IsNumericInput(inputBuf)) {
                            ConsoleWrite("[ERROR] Please enter a valid number!\r\n");
                            ConsoleWrite("Press Enter to continue...");
                            ConsoleRead(inputBuf, sizeof(inputBuf));
                        } else {
                            int newLen = SimpleStrToInt(inputBuf);
                            if (newLen >= 0 && newLen < MAX_CATEGORY_LENGTH) {
                                symbolLength = newLen;
                            } else {
                                wsprintfA(displayBuf, "[ERROR] Length must be between 0 and %d!\r\n", MAX_CATEGORY_LENGTH - 1);
                                ConsoleWrite(displayBuf);
                                ConsoleWrite("Press Enter to continue...");
                                ConsoleRead(inputBuf, sizeof(inputBuf));
                            }
                        }
                    }
                    break;
                }
                
                case 8:
                    running = FALSE;
                    break;
            }
        }
        /* If readLen == 0 (empty input/just Enter), silently refresh menu */
    }
    
    /* Clean exit message */
    ClearScreen();
    ConsoleWrite("Goodbye.\r\n");
}