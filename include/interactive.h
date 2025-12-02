/**
 * @file interactive.h
 * @brief Interactive menu mode for password generation
 * @details Provides a user-friendly menu interface for configuring and generating
 *          passwords when no command-line arguments are provided.
 */

#ifndef INTERACTIVE_H
#define INTERACTIVE_H

#include "common.h"

/**
 * @brief Runs the interactive menu mode with full configuration options
 * @details Displays a menu allowing users to:
 *          - Toggle character categories (letters, numbers, symbols)
 *          - Configure length per category
 *          - Generate passwords with current settings
 *          - Exit the program
 *          Loops until user selects exit option.
 */
void RunInteractiveMode();

#endif