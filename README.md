# WinPass-Native

A lightweight, cryptographically secure password generator for Windows. Built using pure Win32 API without any standard C library dependencies.

![Version](https://img.shields.io/badge/version-2.0.0-blue)
![Platform](https://img.shields.io/badge/platform-Windows-lightgrey)
![License](https://img.shields.io/badge/license-MIT-green)

## Features

- **Cryptographically Secure** - Uses Windows CryptoAPI (`CryptGenRandom`) for random number generation
- **No Standard Library** - Pure Win32 API implementation (no `stdio.h` or `stdlib.h`)
- **Fisher-Yates Shuffle** - Implements unbiased shuffling with Rejection Sampling to eliminate Modulo Bias
- **Automatic Clipboard** - Generated passwords are automatically copied to clipboard
- **Three Operation Modes** - Legacy batch, Advanced CLI, and Interactive menu

## Installation

### Prerequisites

- Windows operating system (Windows 2000 or later)
- GCC compiler (MinGW-w64 recommended)

### Building from Source

```batch
git clone https://github.com/YudaKusumaID/WinPass-Native.git
cd WinPass-Native
build.bat
```

This produces `WinPass.exe` in the project directory.

## Usage

### Interactive Mode (Default)

Run without arguments to launch the interactive menu:

```batch
WinPass.exe
```

The menu provides options to:
- Generate passwords
- Toggle character categories (Letters, Numbers, Symbols)
- Set length for each category
- View current configuration

### Legacy Batch Mode

Generate a password with a single length argument:

```batch
WinPass.exe 20
```

Generates a 20-character password using all character types.

### Advanced CLI Mode

Fine-grained control using flags:

```batch
# Custom lengths per category (8 letters + 4 numbers + 4 symbols)
WinPass.exe --letters=8 --numbers=4 --symbols=4

# Short form
WinPass.exe -l=10 -n=5 -s=5

# Disable specific categories
WinPass.exe --no-symbols --letters=16 --numbers=4

# Letters only
WinPass.exe --no-numbers --no-symbols --letters=24
```

#### Available Flags

| Flag | Short | Description |
|------|-------|-------------|
| `--letters=N` | `-l=N` | Set number of letters |
| `--numbers=N` | `-n=N` | Set number of digits |
| `--symbols=N` | `-s=N` | Set number of symbols |
| `--no-letters` | - | Disable letters |
| `--no-numbers` | - | Disable numbers |
| `--no-symbols` | - | Disable symbols |
| `--help` | `-h` | Show help |

## Character Sets

| Category | Characters | Count |
|----------|-----------|-------|
| Letters | `a-z`, `A-Z` | 52 |
| Numbers | `0-9` | 10 |
| Symbols | `!@#$%^&*()-_=+[]{}` etc. | 22 |

## Project Structure

```
WinPass-Native/
├── main.c                 # Entry point and mode detection
├── build.bat              # Build script
├── include/
│   ├── common.h           # Platform includes and charset declarations
│   ├── cli_parser.h       # CLI argument parsing interface
│   ├── console_io.h       # Console I/O operations
│   ├── interactive.h      # Interactive mode interface
│   ├── password_gen.h     # Password generation interface
│   └── utils.h            # Utility functions
└── src/
    ├── charset.c          # Character set definitions
    ├── cli_parser.c       # CLI argument parsing implementation
    ├── console_io.c       # Console read/write operations
    ├── interactive.c      # Interactive menu implementation
    ├── password_gen.c     # Core password generation logic
    └── utils.c            # String and number utilities
```

## Technical Details

### Security Considerations

- **CryptoAPI**: Uses `CryptGenRandom` for cryptographically secure random byte generation
- **Rejection Sampling**: Eliminates Modulo Bias in the Fisher-Yates shuffle algorithm, ensuring uniform distribution of all possible permutations
- **No Standard Library**: Reduces attack surface by avoiding CRT dependencies

### Dependencies

The executable links against:
- `Advapi32.dll` - Cryptographic functions
- `Shell32.dll` - Command line parsing

## License

This project is licensed under the MIT License.

## Author
The author of this project is Yuda Kusuma.
Developed for the DDP (Dasar-Dasar Pemrograman) course project.
