/**
 * @file console_io.c
 * @brief Console I/O operations implementation using Win32 API
 * @details Implements console interaction without standard C library (no printf/scanf).
 *          All operations use Windows Console API directly for better control.
 */

#include "../include/console_io.h"

/**
 * @brief Writes ASCII string to console output
 * @param str Null-terminated string to write
 */
void ConsoleWrite(const char* str) {
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD bytesWritten;
    if (hStdOut != INVALID_HANDLE_VALUE) {
        WriteFile(hStdOut, str, lstrlenA(str), &bytesWritten, NULL);
    }
}

/**
 * @brief Reads user input from console with CRLF handling
 * @param buffer Buffer to store input
 * @param maxLen Maximum buffer size
 * @return Number of characters read (excluding CRLF and null terminator)
 */
int ConsoleRead(char* buffer, int maxLen) {
    HANDLE hStdIn = GetStdHandle(STD_INPUT_HANDLE);
    DWORD bytesRead;

    if (ReadConsoleA(hStdIn, buffer, maxLen - 1, &bytesRead, NULL)) {
        /* Windows console returns CRLF (\r\n) at end of line, need to strip it */
        if (bytesRead >= 2 && buffer[bytesRead-2] == '\r') {
            buffer[bytesRead-2] = '\0';  /* Replace \r with null terminator */
            return bytesRead - 2;        /* Return length without CRLF */
        }
        buffer[bytesRead] = '\0';
        return bytesRead;
    }
    return 0;
}

/**
 * @brief Prints formatted error message with GetLastError() code
 * @param msg Error description
 */
void PrintError(const char* msg) {
    char buf[256];
    wsprintfA(buf, "[ERROR] %s (Code: %lu)\r\n", msg, GetLastError());
    ConsoleWrite(buf);
}

/**
 * @brief Clears console screen using Win32 buffer manipulation
 * @details Avoids system("cls") by directly manipulating console screen buffer.
 *          More efficient and doesn't spawn external process.
 */
void ClearScreen() {
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD count;
    DWORD cellCount;
    COORD homeCoords = { 0, 0 };

    if (hStdOut == INVALID_HANDLE_VALUE) return;

    if (!GetConsoleScreenBufferInfo(hStdOut, &csbi)) return;
    cellCount = csbi.dwSize.X * csbi.dwSize.Y;  /* Total character cells in buffer */

    /* Fill entire buffer with spaces to clear visible content */
    if (!FillConsoleOutputCharacter(hStdOut, (TCHAR)' ', cellCount, homeCoords, &count)) return;

    /* Restore default attributes (color) for entire buffer */
    if (!FillConsoleOutputAttribute(hStdOut, csbi.wAttributes, cellCount, homeCoords, &count)) return;

    /* Reset cursor to top-left corner (home position) */
    SetConsoleCursorPosition(hStdOut, homeCoords);
}
