#include <stdio.h>
#include <time.h>
#include "freecell.h"

int main() {
    int gameResult;

    start:
    gameResult = initGame();

    switch (gameResult) {
        case 0: // losing game
            // print losing screen
            gameWonScreen();
            if (askNewGame())
                goto start;
            break;
        case 1: // winning game
            gameLostScreen();
            if (askNewGame())
                goto start;
            break;
        case 2: // New game
            goto start;
        case 3: // End game
            return 0;
        default:
            break;
    }

    exit:
    return 0;
}
