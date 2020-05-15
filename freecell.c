//
// Created by haita on 2020/05/07.
//

#include "freecell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>

void initGame() {

    char src, dest;
    Deck *src_deck, *dest_deck;

    srand((unsigned) time(NULL));

    Zone *zone1 = createZone(ZONE1_SIZE),
            *zone2 = createZone(ZONE23_SIZE),
            *zone3 = createZone(ZONE23_SIZE);

    fillZone1(zone1, 30);

    printLayout(zone1, zone2, zone3);

    // TODO: move card depending on use input
    // Best case scenario
    while (TRUE) {
        //Clearing input buffer (aka trailing '\n')
        printf("\nNext move (src dest): ");
        scanf("%c %c", &src, &dest);
        clearBuffer();

        // Get the trailing '\n'
        if (src == '0')
            break;
        src_deck = getDeck(zone1, zone2, src);
        dest_deck = getDeck(zone1, zone2, dest);

        printf("\nMoving result : %d\n", moveCard(src_deck, dest_deck));

        printLayout(zone1, zone2, zone3);
    }
}

Deck *createDeck(size_t capacity) {
    Deck *deck = malloc(sizeof(Deck));
    deck->capacity = capacity;
    deck->size = 0;
    return deck;
}

void fillInitialDeck(Deck *deck) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 13; j++) {
            addCard(createCard(i, j), deck);
        }
    }
}

void fillDeckFrom(Deck *srcDeck, Deck *destDeck) {
    if (!srcDeck || !srcDeck->size)
        return;

    size_t i = 0;
    Card *pCard = srcDeck->head;

    destDeck->head = pCard;

    for (; (i < (destDeck->capacity - 1)) && (i < srcDeck->size); i++) {
        pCard = pCard->next;
    }

    srcDeck->head = pCard->next;
    srcDeck->size -= ++i;
    pCard->next = NULL;
    destDeck->tail = pCard;
    destDeck->size = i;
}

Deck *getDeck(Zone *zone1, Zone *zone2, char position) {
    int deck_index = strchr(ZONE1_CONTROLS ZONE2_CONTROLS, position) - (ZONE1_CONTROLS ZONE2_CONTROLS);

    if (deck_index < 0)
        return NULL;

    if (deck_index < ZONE1_SIZE)
        return zone1->decks[deck_index];

    return zone2->decks[deck_index - ZONE1_SIZE];
}

Card *createCard(Card_Type type, Card_number number) {

    Card *card = malloc(sizeof(Card *));
    card->number = number;
    card->type = type;
    card->next = NULL;
    card->cardShape = (CardShape *) malloc(sizeof(CardShape *));
    createCardShape(card);

    return card;
}

char *getNumber(Card_number cardNumber) {
    char n;
    switch (cardNumber) {

        case A:
            return "A";
        case Two:
            return "2";
        case Three:
            return "3";
        case Four:
            return "4";
        case Five:
            return "5";
        case Six:
            return "6";
        case Seven:
            return "7";
        case Eight:
            return "8";
        case Nine:
            return "9";
        case Ten:
            return "10";
        case J:
            return "J";
        case Q:
            return "Q";
        case K:
            return "K";
        default:
            return "X";
    }
}

char *getType(Card_Type cardType) {
    switch (cardType) {
        case Clubs:
            return "♣";

        case Diamonds:
            return ANSI_COLOR_RED"♦"ANSI_COLOR_RESET;

        case Hearts:
            return ANSI_COLOR_RED"♥"ANSI_COLOR_RESET;

        case Spades:
            return "♠";

        default:
            return "$";
    }
}

void createCardShape(Card *card) {
    char *n = getNumber(card->number),
            *s = getType(card->type);

    char *middle1 = malloc(sizeof(char) * CARD_WIDTH),
            *middle2 = malloc(sizeof(char) * CARD_WIDTH);

    assert(sprintf(middle1, "│%s %2s│", s, n) > 0);
    assert(sprintf(middle2, "│   %s│", s) > 0);

    card->cardShape->top = CARD_TOP;
    card->cardShape->middle1 = middle1;
    card->cardShape->middle2 = middle2;
    card->cardShape->bottom = CARD_BOTTOM;
}

void printCardShape(Card *card) {
    printf("%s\n%s\n%s\n%s\n",
           card->cardShape->top,
           card->cardShape->middle1,
           card->cardShape->middle2,
           card->cardShape->bottom);
}

bool addCard(Card *card, Deck *deck) {
    if (deck->size >= deck->capacity)
        return FALSE;

    if (deck->size == 0) {
        deck->head = card;
        deck->tail = card;
        deck->head->next = NULL;
        deck->size++;
        return TRUE;
    }

    Card *p_card = cardAt(deck, deck->size - 1);

    p_card->next = card;
    deck->tail = card;
    deck->size++;
    return TRUE;

}

void shuffleDeck(Deck *deck, size_t times) {

    if (!deck->size)
        return;
    int n = deck->size, j = 0, prevj = j;

    while (--times) {

        for (int i = 0; i < n; i++) {

            while (j == i || j == prevj || j < 0)
                j = rand() % n - 1;

            swap(&deck->head, cardAt(deck, i), i ? cardAt(deck, i - 1) : NULL, cardAt(deck, j), cardAt(deck, j - 1));

            prevj = j;  // previously swapped index
        }
    }

    deck->tail = cardAt(deck, n - 1);
}

Card *cardAt(Deck *deck, size_t index) {
    if (index > deck->size)
        return NULL;

    Card *p_card = malloc(sizeof(Card));

    p_card = deck->head;

    for (int j = 0; j < index; j++) {
        p_card = p_card->next;
    }
    return p_card;
}

//TODO: 1- Check if destination or source is zone 2 or 1, else return MOVE_ILLEGAL,
// should be done before calling this function, where we know which zone is source or destination
// 2- case of moving multiple cards at once
int moveCard(Deck *src, Deck *dest) {
    // check if destination if full, if so return DESTINATION_FULL
    if (dest->size == dest->capacity)
        return DESTINATION_FULL;

    if (!dest->size || isCompatible(src->tail, dest->tail)) {
        plainMoveCard(src, dest);
        return SUCCESS;
    }
    // update size of 2 decks
    return FAIL;
}

void plainMoveCard(Deck *src, Deck *dest) {
    if (!dest->size)
        dest->head = src->tail;
    dest->tail = src->tail;
    dest->size++;
    cardAt(src, src->size - 2)->next = NULL;
    if (dest->size > 1)
        cardAt(dest, dest->size - 2)->next = dest->tail;
    src->tail = cardAt(src, src->size - 2);
    src->size--;
}

bool isCompatible(Card *card1, Card *card2) {
    return ((card1->type < 2 && card2->type >= 2)
            || (card1->type >= 2 && card2->type < 2))
           && card1->number == (card2->number - 1);
}

Zone *createZone(size_t zone_size) {
    Zone *zone = malloc(sizeof(Zone));
    zone->size = zone_size;
    zone->decks = malloc(sizeof(Deck *) * zone_size);

    for (int i = 0; i < zone->size; i++) {
        zone->decks[i] = createDeck(zone_size == ZONE1_SIZE ? (i < ZONE1_SIZE / 2 ? 7 : 6) : 1);
    }
    return zone;
}

void swap(Card **head, Card *card1, Card *prevCard1, Card *card2, Card *prevCard2) {

    if (prevCard1) prevCard1->next = card2;
    if (prevCard2) prevCard2->next = card1;


    Card *temp = NULL;
    temp = card1->next;
    card1->next = card2->next;
    card2->next = temp;


    if ((*head) == card1)
        *head = card2;
    else if ((*head) == card2)
        *head = card1;

}

void printDeck(Deck *deck) {
    Card *pCard = deck->head;
    while (pCard) {
        if (pCard->next)
            printf("%s\n%s\n", pCard->cardShape->top, pCard->cardShape->middle1);
        else
            printCardShape(pCard);
        pCard = pCard->next;
    }
    free(pCard);
}

void fillZone1(Zone *zone, size_t shuffles) {
    Deck *deck = createDeck(DECK_CAPACITY);
    fillInitialDeck(deck);

    shuffleDeck(deck, shuffles);

    for (int i = 0; i < ZONE1_SIZE; i++) {
        fillDeckFrom(deck, zone->decks[i]);
        zone->decks[i]->capacity = SET_CAPACITY;
    }

}

void printZone1(Zone *zone) {
    Card *pCard = NULL;
    Deck *currDeck = NULL;
    int skips[ZONE1_SIZE] = {0, 0, 0, 0, 0, 0, 0, 0},
            isEmpty[ZONE1_SIZE] = {0, 0, 0, 0, 0, 0, 0, 0},
            newLine = 1;
    Card *pCards[ZONE1_SIZE];
    size_t max_size = getTopSize(zone);

    for (size_t i = 0; i < ZONE1_SIZE; i++) {
        pCards[i] = zone->decks[i]->head;
    }

    for (size_t i = 0; i < max_size; i++) {
        if (!newLine)
            break;
        for (int k = 0; k < 4 && newLine; k++) {
            newLine = 0;
            for (int j = 0; j < ZONE1_SIZE; j++) {
                if (!j)
                    printf(ZONE1_PADDING);

                currDeck = zone->decks[j];

                if (!currDeck->size)
                    isEmpty[j] = 1;

                pCard = pCards[j];

                if (!pCard && !isEmpty[j]) {
                    if (k == (max_size / 2))    // Not print new line when all cards are printed
                        newLine = 0;
                    printf("\t    ");
                    continue;
                }

                newLine = 1;

                switch (k) {
                    case 0:
                        if (isEmpty[j]) {
                            if (!i)
                                printf("\t%s", CARD_TOP);
                            else
                                printf("\t    ");
                            break;
                        }
                        if (i * 2 >= currDeck->size) {
                            printf("\t%s", pCard->cardShape->middle2);
                        } else
                            printf("\t%s", CARD_TOP);
                        skips[j] = 0;
                        break;
                    case 1:
                        if (isEmpty[j]) {
                            if (!i)
                                printf("\t%s", CARD_PLACEHOLDER_EMPTY.middle1);
                            else {
                                printf("\t    ");
                            }
                            break;
                        }
                        if (i * 2 >= currDeck->size) {
                            printf("\t%s", CARD_BOTTOM);
                            pCards[j] = NULL;
                        } else
                            printf("\t%s", pCard->cardShape->middle1);
                        skips[j] = 0;
                        break;
                    case 2:
                        if (isEmpty[j]) {
                            if (!i)
                                printf("\t%s", CARD_PLACEHOLDER_EMPTY.middle2);
                            else{
                                printf("\t    ");
                            }
                            break;
                        }
                        if (pCard->next) {
                            printf("\t%s", CARD_TOP);
                        } else
                            printf("\t%s", pCard->cardShape->middle2);
                        break;
                    case 3:
                        if (isEmpty[j]) {
                            if (!i)
                                printf("\t%s", CARD_BOTTOM);
                            else
                                printf("\t    ");
                            break;
                        }
                        if (!i && isEmpty[j]) {
                            printf("\t%s", CARD_BOTTOM);
                            break;
                        }
                        if (pCard->next) {
                            printf("\t%s", pCard->next->cardShape->middle1);
                            if (pCard->next->next)
                                pCards[j] = pCard->next->next;
                            else if (pCard->next)
                                pCards[j] = pCard->next;
                            skips[j] = 1;
                        } else
                            printf("\t%s", CARD_BOTTOM);
                        break;
                    default:
                        break;
                }
                fflush(stdout);
            }

            printf("\n");
        }
        for (int s = 0; s < ZONE1_SIZE; s++) {
            if (pCards[s]) {
                if (!skips[s])
                    pCards[s] = pCards[s]->next;
                skips[s] = 0;
            }
        }

    }

    printf(ZONE1_PADDING);
    for (int i = 0; i < ZONE1_SIZE; i++) {
        printf("\t  %c ", ZONE1_CONTROLS[i]);
    }

    free(pCard);
}

void printZone23(Zone *zone2, Zone *zone3) {
    Card *pCard = NULL;
    Deck *currDeck = NULL;
    int skips[ZONE1_SIZE] = {0, 0, 0, 0, 0, 0, 0, 0},
            isEmpty[ZONE1_SIZE] = {0, 0, 0, 0, 0, 0, 0, 0},
            newLine = 1;
    Card *pCards[ZONE23_SIZE * 2];

    for (int i = 0; i < ZONE23_SIZE * 2; i++) {
        if (i < ZONE23_SIZE)
            pCards[i] = zone2->decks[i]->head;
        else
            pCards[i] = zone3->decks[i - ZONE23_SIZE]->head;
    }

    for (int i = 0; i < 1; i++) {
        for (int k = 0; k < 4 && newLine; k++) {
            newLine = 0;
            for (int j = 0; j < ZONE23_SIZE * 2; j++) {

                if (!j)
                    printf(ZONE23_PADDING);
                else if (j == ZONE23_SIZE)
                    printf(ZONE23_PADDING ZONE23_PADDING);

                if (j < ZONE23_SIZE)
                    currDeck = zone2->decks[j];
                else
                    currDeck = zone3->decks[j - ZONE23_SIZE];

                if (currDeck->size == 0)
                    isEmpty[j] = 1;

                pCard = pCards[j];

                if (!pCard && !isEmpty[j]) {
                    if (k == 3)                     // Not print new line only if bottom of card is displayed
                        newLine = 0;
                    printf("\t    ");
                    continue;
                }

                newLine = 1;

                switch (k) {
                    case 0:
                        if (isEmpty[j]) {
                            if (!i)
                                printf("\t%s", CARD_TOP);
                            else
                                printf("\t    ");
                            break;
                        }
                        if (i * 2 >= currDeck->size) {
                            printf("\t%s", pCard->cardShape->middle2);
                        } else
                            printf("\t%s", pCard->cardShape->top);
                        skips[j] = 0;
                        break;
                    case 1:
                        if (isEmpty[j]) {
                            if (!i)
                                printf("\t%s",
                                       j < ZONE23_SIZE ? CARD_PLACEHOLDER_EMPTY.middle1 : CARD_PLACEHOLDER.middle1);
                            else {
                                printf("\t    ");
                            }
                            break;
                        }
                        if (i * 2 >= currDeck->size) {
                            printf("\t%s", CARD_BOTTOM);
                            pCards[j] = NULL;
                        } else
                            printf("\t%s", pCard->cardShape->middle1);
                        skips[j] = 0;
                        break;
                    case 2:
                        if (isEmpty[j]) {
                            if (!i)
                                printf("\t%s",
                                       j < ZONE23_SIZE ? CARD_PLACEHOLDER_EMPTY.middle2 : CARD_PLACEHOLDER.middle2);
                            else{
                                printf("\t    ");
                            }
                            break;
                        }
                        if (pCard->next) {
                            printf("\t%s", CARD_TOP);
                        } else
                            printf("\t%s", pCard->cardShape->middle2);
                        break;
                    case 3:
                        if (isEmpty[j]) {
                            if (!i)
                                printf("\t%s", CARD_BOTTOM);
                            else
                                printf("\t    ");
                            continue;
                        }
                        if (pCard->next) {
                            printf("\t%s", pCard->next->cardShape->middle1);
                            if (pCard->next->next)
                                pCards[j] = pCard->next->next;
                            else if (pCard->next)
                                pCards[j] = pCard->next;
                            skips[j] = 1;
                        } else
                            printf("\t%s", CARD_BOTTOM);
                        break;
                    default:
                        continue;
                }
                fflush(stdout);
            }
            printf("\n");
        }
        for (int s = 0; s < ZONE1_SIZE; s++) {
            if (pCards[s]) {
                if (!skips[s])
                    pCards[s] = pCards[s]->next;
                skips[s] = 0;
            }
        }

    }

    printf(ZONE23_PADDING);
    for (int i = 0; i < ZONE23_SIZE * 2; i++) {
        if (i == 4)
            printf(ZONE23_PADDING ZONE23_PADDING);
        if (i < ZONE23_SIZE)
            printf("\t  %c ", ZONE2_CONTROLS[i]);
        else
            printf("\t  %s ", getType(i - ZONE23_SIZE));
    }

    free(pCard);
}

void printLayout(Zone *zone1, Zone *zone2, Zone *zone3) {
    clrscr();
    printZone23(zone2, zone3);
    printf("\n\n\n");
    printZone1(zone1);
}

size_t getTopSize(Zone *zone) {
    size_t max = 0;
    for (int i = 0; i < zone->size; i++) {
        if (zone->decks[i]->size > max)
            max = zone->decks[i]->size;
    }
    return max;
}
