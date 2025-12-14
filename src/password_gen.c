/**
 * @file password_gen.c
 * @brief Password generation core logic implementation
 * @details Implements cryptographically secure password generation using Windows
 *          CryptoAPI (CryptGenRandom). Supports both simple and advanced multi-charset
 *          generation modes with Fisher-Yates shuffling for uniform distribution.
 */

#include "../include/password_gen.h"
#include "../include/console_io.h"

/**
 * @brief Copies generated password to Windows clipboard
 * @param text Password string to copy
 * @param length Length of password (without null terminator)
 */
void CopyToClipboard(const char* text, int length) {
    if (!OpenClipboard(NULL)) return;
    EmptyClipboard();
    
    /* Allocate moveable global memory for clipboard data */
    HGLOBAL hGlob = GlobalAlloc(GMEM_MOVEABLE, length + 1);
    if (hGlob) {
        char* pData = (char*)GlobalLock(hGlob);
        if (pData) {
            for(int i=0; i<length; i++) pData[i] = text[i];
            pData[length] = 0;  /* Null terminator */
            GlobalUnlock(hGlob);
            
            /* Transfer ownership to clipboard; if successful, don't free hGlob */
            if (!SetClipboardData(CF_TEXT, hGlob)) GlobalFree(hGlob);
            else ConsoleWrite("[INFO] Copied to Clipboard.\r\n");
        } else {
            /* GlobalLock failed - must free allocated memory */
            GlobalFree(hGlob);
        }
    }
    CloseClipboard();
}

/**
 * @brief Shuffles password characters using Fisher-Yates algorithm with Rejection Sampling
 * @param password Password string to shuffle in-place
 * @param length Length of password
 * @param hCryptProv Cryptographic context for secure random bytes
 * @details Implements cryptographically secure shuffling by eliminating Modulo Bias.
 *          Uses Rejection Sampling: discards random values that would cause non-uniform
 *          distribution, ensuring all permutations have exactly equal probability.
 *          
 *          Security Note: Direct modulo operation (rand % n) creates bias when the
 *          random source range (MAXDWORD) is not evenly divisible by n. This bias,
 *          though small, reduces entropy and could theoretically aid attackers in
 *          optimizing brute-force strategies by targeting more probable permutations.
 */
void ShufflePassword(char* password, int length, HCRYPTPROV hCryptProv) {
    /* 
     * Fisher-Yates shuffle algorithm (modern variant) with Rejection Sampling
     * Uses 32-bit random values (DWORD) for better distribution characteristics
     * Iterates backwards from last element to second element
     */
    for (int i = length - 1; i > 0; i--) {
        DWORD dwRange = (DWORD)(i + 1);  /* Range size: [0, i] = i+1 possibilities */
        DWORD dwThreshold;
        DWORD dwRandomValue;
        
        /*
         * Calculate rejection threshold to eliminate Modulo Bias
         * 
         * Concept: MAXDWORD (4,294,967,295) % dwRange gives us the "remainder"
         * values that would cause uneven distribution. By rejecting random values
         * >= threshold, we ensure only values that map evenly to [0, dwRange) are used.
         * 
         * Example: For dwRange=60, MAXDWORD=4294967295
         *   - 4294967295 % 60 = 16 (there are 16 "leftover" values)
         *   - threshold = 4294967295 - 16 = 4294967279
         *   - Values [0, 4294967279] map evenly (each target gets 71582788 sources)
         *   - Values [4294967280, 4294967295] would cause bias (rejected)
         */
        dwThreshold = MAXDWORD - (MAXDWORD % dwRange);

        /*
         * Rejection Sampling Loop
         * Generate random DWORD values until we get one below threshold.
         * Expected iterations: ~1.0 (bias is extremely small for typical ranges)
         * Worst case: For range=1, no rejection needed. For range=MAXDWORD, ~1.0.
         */
        do {
            if (!CryptGenRandom(hCryptProv, sizeof(DWORD), (BYTE*)&dwRandomValue)) {
                /* Cryptographic failure - abort shuffle to avoid weak randomness */
                return;
            }
        } while (dwRandomValue >= dwThreshold);
        
        /*
         * Now safe to use modulo: dwRandomValue is guaranteed to be < threshold,
         * so modulo operation produces perfectly uniform distribution across [0, i]
         */
        int j = (int)(dwRandomValue % dwRange);
        
        /* Swap password[i] with password[j] */
        char temp = password[i];
        password[i] = password[j];
        password[j] = temp;
    }
}

/**
 * @brief Generates password with simple configuration (legacy/batch mode)
 * @param length Total password length
 * @param useSymbols TRUE to include symbols, FALSE for alphanumeric only
 */
void GenerateCore(int length, BOOL useSymbols) {
    HCRYPTPROV hCryptProv = 0;
    HANDLE hHeap = GetProcessHeap();
    BYTE* pbBuffer = NULL;
    char* passwordString = NULL;

    const char* currentCharset = useSymbols ? CHARSET_FULL : CHARSET_ALPHANUM;
    int charsetLen = lstrlenA(currentCharset);
    /* Buffer sized for max password + formatting overhead */
    char msgBuf[MAX_PASSWORD_LENGTH + 64];

    if (length < MIN_PASSWORD_LENGTH) {
        wsprintfA(msgBuf, "\r\n[ERROR] Password length must be at least %d characters!\r\n", MIN_PASSWORD_LENGTH);
        ConsoleWrite(msgBuf);
        ConsoleWrite("Press Enter to continue...");
        char dummy[10];
        ConsoleRead(dummy, sizeof(dummy));
        return;
    }

    pbBuffer = (BYTE*)HeapAlloc(hHeap, HEAP_ZERO_MEMORY, length);
    if (!pbBuffer) {
        PrintError("Memory Error");
        return;
    }
    
    passwordString = (char*)HeapAlloc(hHeap, HEAP_ZERO_MEMORY, length + 1);
    if (!passwordString) {
        HeapFree(hHeap, 0, pbBuffer);  /* Clean up first allocation */
        PrintError("Memory Error");
        return;
    }

    /* Acquire cryptographic context for secure random generation */
    if (CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT)) {
        if (CryptGenRandom(hCryptProv, length, pbBuffer)) {
            /* Map random bytes to charset using modulo (acceptable bias for large charsets) */
            for (int i = 0; i < length; i++) {
                passwordString[i] = currentCharset[pbBuffer[i] % charsetLen];
            }
            passwordString[length] = '\0';

            wsprintfA(msgBuf, "\r\n>> RESULT (%d chars): %s\r\n", length, passwordString);
            ConsoleWrite(msgBuf);
            CopyToClipboard(passwordString, length);
        } else {
            PrintError("GenRandom Failed");
        }
        CryptReleaseContext(hCryptProv, 0);
    } else {
        PrintError("Crypto Context Failed");
    }

    HeapFree(hHeap, 0, pbBuffer);
    HeapFree(hHeap, 0, passwordString);
}

/**
 * @brief Generates password with advanced per-category configuration
 * @param letterCount Number of letter characters
 * @param numberCount Number of numeric characters
 * @param symbolCount Number of symbol characters
 * @param useLetters Enable/disable letters category
 * @param useNumbers Enable/disable numbers category
 * @param useSymbols Enable/disable symbols category
 */
void GenerateAdvanced(int letterCount, int numberCount, int symbolCount,
                      BOOL useLetters, BOOL useNumbers, BOOL useSymbols) {
    HCRYPTPROV hCryptProv = 0;
    HANDLE hHeap = GetProcessHeap();
    BYTE* pbBuffer = NULL;
    char* passwordString = NULL;
    /* Buffer sized for max password + formatting overhead */
    char msgBuf[MAX_PASSWORD_LENGTH + 128];

    /* Validate that at least one category is enabled */
    if (!useLetters && !useNumbers && !useSymbols) {
        ConsoleWrite("\r\n[ERROR] At least one character type must be enabled!\r\n");
        ConsoleWrite("Press Enter to continue...");
        char dummy[10];
        ConsoleRead(dummy, sizeof(dummy));
        return;
    }

    /* Calculate total password length from enabled categories */
    int totalLength = 0;
    if (useLetters) totalLength += letterCount;
    if (useNumbers) totalLength += numberCount;
    if (useSymbols) totalLength += symbolCount;

    /* Validate minimum password length for security */
    if (totalLength < MIN_PASSWORD_LENGTH) {
        wsprintfA(msgBuf, "\r\n[ERROR] Password length must be at least %d characters!\r\n", MIN_PASSWORD_LENGTH);
        ConsoleWrite(msgBuf);
        ConsoleWrite("Press Enter to continue...");
        char dummy[10];
        ConsoleRead(dummy, sizeof(dummy));
        return;
    }

    pbBuffer = (BYTE*)HeapAlloc(hHeap, HEAP_ZERO_MEMORY, totalLength);
    if (!pbBuffer) {
        PrintError("Memory Error");
        return;
    }
    
    passwordString = (char*)HeapAlloc(hHeap, HEAP_ZERO_MEMORY, totalLength + 1);
    if (!passwordString) {
        HeapFree(hHeap, 0, pbBuffer);  /* Clean up first allocation */
        PrintError("Memory Error");
        return;
    }

    if (CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT)) {
        if (CryptGenRandom(hCryptProv, totalLength, pbBuffer)) {
            int pos = 0;  /* Current write position in password string */

            /* 
             * Phase 1: Assemble password from separate character categories
             * Each category uses its own section of the random buffer
             */
            
            if (useLetters && letterCount > 0) {
                int lettersLen = lstrlenA(CHARSET_LETTERS);
                /* Use first letterCount bytes of random buffer for letters */
                for (int i = 0; i < letterCount; i++) {
                    passwordString[pos++] = CHARSET_LETTERS[pbBuffer[i] % lettersLen];
                }
            }

            if (useNumbers && numberCount > 0) {
                int numbersLen = lstrlenA(CHARSET_NUMBERS);
                /* Offset into random buffer to avoid reusing bytes from letters */
                int offset = useLetters ? letterCount : 0;
                for (int i = 0; i < numberCount; i++) {
                    passwordString[pos++] = CHARSET_NUMBERS[pbBuffer[offset + i] % numbersLen];
                }
            }

            if (useSymbols && symbolCount > 0) {
                int symbolsLen = lstrlenA(CHARSET_SYMBOLS);
                /* Calculate offset to skip previously used random bytes */
                int offset = 0;
                if (useLetters) offset += letterCount;
                if (useNumbers) offset += numberCount;
                for (int i = 0; i < symbolCount; i++) {
                    passwordString[pos++] = CHARSET_SYMBOLS[pbBuffer[offset + i] % symbolsLen];
                }
            }

            passwordString[totalLength] = '\0';

            /*
             * Phase 2: Shuffle to eliminate predictable category ordering
             * Without shuffling, password would be [letters][numbers][symbols]
             */
            ShufflePassword(passwordString, totalLength, hCryptProv);

            wsprintfA(msgBuf, "\r\n>> RESULT (%d chars: L=%d N=%d S=%d): %s\r\n",
                      totalLength,
                      useLetters ? letterCount : 0,
                      useNumbers ? numberCount : 0,
                      useSymbols ? symbolCount : 0,
                      passwordString);
            ConsoleWrite(msgBuf);
            CopyToClipboard(passwordString, totalLength);
            
            ConsoleWrite("\r\nPress Enter to continue...");
            char dummy[10];
            ConsoleRead(dummy, sizeof(dummy));
        } else {
            PrintError("GenRandom Failed");
        }
        CryptReleaseContext(hCryptProv, 0);
    } else {
        PrintError("Crypto Context Failed");
    }

    HeapFree(hHeap, 0, pbBuffer);
    HeapFree(hHeap, 0, passwordString);
}