#include <stdio.h>
#include <time.h>

#include "freecell.h"

int main() {

    srand((unsigned) time(NULL));

    Deck *deck = createDeck(DECK_CAPACITY), *deck2 = createDeck(DECK_CAPACITY);
    fillInitialDeck(deck);
    fillInitialDeck(deck2);

    shuffleDeck(deck, 10);
    shuffleDeck(deck2, 10);

    Zone *zone1 = createZone(ZONE1_SIZE),
            *zone2 = createZone(ZONE23_SIZE),
            *zone3 = createZone(ZONE23_SIZE);

    for (int i = 0; i < ZONE1_SIZE; i++) {
        fillDeckFrom(deck, zone1->decks[i]);
    }


    for (int i = 0; i < ZONE1_SIZE; i++) {
        if (i < ZONE23_SIZE) {
            zone2->decks[i]->capacity /=
                    2; // lowering down the capacity so that all the cards can fit in 2 zones for now
            fillDeckFrom(deck2, zone2->decks[i]);
        } else {
            zone3->decks[i - ZONE23_SIZE]->capacity /=
                    2; // lowering down the capacity so that all the cards can fit in 2 zones for now
            fillDeckFrom(deck2, zone3->decks[i - ZONE23_SIZE]);
        }

    }

    zone1->decks[1]->size = 0;
    zone1->decks[3]->size = 0;
    zone1->decks[5]->head->next->next = NULL;

    zone2->decks[1]->size = 0;
    zone3->decks[2]->size = 0;
    zone2->decks[3]->head->next->next = NULL;

    printZone23(zone2, zone3);

    printf("\n\n\n\n\n");

    printZone1(zone1);

    return 0;
}
