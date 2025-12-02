/**
 * @file console_io.h
 * @brief Console input/output operations using Win32 API
 * @details Provides console I/O functions that use Win32 API (WriteFile, ReadConsoleA)
 *          instead of standard C library functions, maintaining the project's pure
 *          Win32 implementation without stdio.h dependency.
 */

#ifndef CONSOLE_IO_H
#define CONSOLE_IO_H

#include "common.h"

/**
 * @brief Writes ASCII string to console output
 * @param str Null-terminated string to write
 * @details Uses Win32 WriteFile API via STD_OUTPUT_HANDLE
 */
void ConsoleWrite(const char* str);

/**
 * @brief Reads user input from console
 * @param buffer Buffer to store input string
 * @param maxLen Maximum buffer size including null terminator
 * @return Number of characters read (excluding CRLF and null terminator)
 * @details Uses ReadConsoleA and automatically strips Windows CRLF line endings
 */
int ConsoleRead(char* buffer, int maxLen);

/**
 * @brief Prints formatted error message with system error code
 * @param msg Error message description
 * @details Appends GetLastError() code to the message for debugging
 */
void PrintError(const char* msg);

/**
 * @brief Clears the console screen and resets cursor position
 * @details Uses Win32 console buffer manipulation to clear screen without system()
 */
void ClearScreen();

#endif