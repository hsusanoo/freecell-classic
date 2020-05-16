#include <stdio.h>
#include <time.h>
#include "freecell.h"

int main() {

    int gameResult;

    start:
    gameResult = initGame();

    switch (gameResult) {
        case 0: // losing game
            gameLostScreen();
            gameWonScreen();
            if (askNewGame())
                goto start;
            break;
        case 1: // winning game
            gameWonScreen();
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

    return 0;
}
