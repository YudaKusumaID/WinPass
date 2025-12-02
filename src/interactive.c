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
        if (ConsoleRead(inputBuf, sizeof(inputBuf)) > 0) {
            char choice = inputBuf[0];  /* First character is the menu selection */

            switch (choice) {
                case '1':
                    /* Generate password with current configuration */
                    GenerateAdvanced(letterLength, numberLength, symbolLength,
                                   useLetters, useNumbers, useSymbols);
                    break;
                    
                /* Toggle options: flip boolean state */
                case '2':
                    useLetters = !useLetters;
                    break;
                case '3':
                    useNumbers = !useNumbers;
                    break;
                case '4':
                    useSymbols = !useSymbols;
                    break;
                    
                /* Length configuration options: prompt for new value */
                case '5': {
                    ClearScreen();
                    ConsoleWrite("Enter letter length: ");
                    ConsoleRead(inputBuf, sizeof(inputBuf));
                    int newLen = SimpleStrToInt(inputBuf);
                    /* Validate range to prevent negative or excessive lengths */
                    if (newLen >= 0 && newLen < 1024) letterLength = newLen;
                    break;
                }
                case '6': {
                    ClearScreen();
                    ConsoleWrite("Enter number length: ");
                    ConsoleRead(inputBuf, sizeof(inputBuf));
                    int newLen = SimpleStrToInt(inputBuf);
                    if (newLen >= 0 && newLen < 1024) numberLength = newLen;
                    break;
                }
                case '7': {
                    ClearScreen();
                    ConsoleWrite("Enter symbol length: ");
                    ConsoleRead(inputBuf, sizeof(inputBuf));
                    int newLen = SimpleStrToInt(inputBuf);
                    if (newLen >= 0 && newLen < 1024) symbolLength = newLen;
                    break;
                }
                
                /* Exit options: support both numeric and 'q' for quit */
                case '8':
                case 'q':
                    running = FALSE;
                    break;
                    
                default:
                    /* Invalid choice: silently ignore and redisplay menu */
                    break;
            }
        }
    }
    
    /* Clean exit message */
    ClearScreen();
    ConsoleWrite("Goodbye.\r\n");
}