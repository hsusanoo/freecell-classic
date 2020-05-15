//
// Created by haita on 2020/05/07.
//

#ifndef FREECELL_UTILS_H
#define FREECELL_UTILS_H

// Printing red color
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_RESET   "\x1b[0m"

// Moving the cursor one line up
#define GO_UP   "\033[F"

// Toggle console ouptut wrap
#define TOGGLE_WRAP_ENABLE "\x1b[7h"
#define TOGGLE_WRAP_DISABLE "\x1b[7l"

#define CARD_WIDTH 6

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__) || defined(__WINDOWS__)
#define CLR "cls"
#else
#define CLR "clear"
#endif

typedef enum {
    FALSE,
    TRUE
} bool;

/**
 * Print in red color
 */
void printr(char *);

//TODO: Placeholder for card

/**
 * Clear screen
 */
void clrscr();

void clearBuffer();

///**
// * Enable console output wrap
// */
//void wrapEnable();
//
///**
// * Disable console output wrap
// */
//void wrapDisable();

#endif //FREECELL_UTILS_H
