//
// Created by haita on 2020/05/07.
//

#ifndef FREECELL_FREECELL_H
#define FREECELL_FREECELL_H

typedef struct Zone Zone;
typedef struct Deck Deck;
typedef struct Card Card;
typedef struct CardShape CardShape;

#define ZONE1_SIZE 8
#define ZONE23_SIZE 4
#define DECK_CAPACITY 52
#define SET_CAPACITY 13
#define CARD_TOP "╭────╮"
#define CARD_BOTTOM "╰────╯"
#define CARD_PLACEHOLDER (CardShape){"╭────╮", "│⢠⣒⡄│", "│⠘⠭⠃│", "╰────╯"}
#define CARD_PLACEHOLDER_EMPTY (CardShape){"╭────╮", "│    │", "│    │", "╰────╯"}
#define ZONE1_CONTROLS  "QSDFGHJK"
#define ZONE2_CONTROLS  "AZER"
#define ZONE3_CONTROLS  "UIOP"

#include <stdlib.h>
#include "utils.h"

typedef enum {
    Clubs,
    Diamonds,
    Hearts,
    Spades
} Card_Type;

typedef enum {
    A,
    Two,
    Three,
    Four,
    Five,
    Six,
    Seven,
    Eight,
    Nine,
    Ten,
    J,
    Q,
    K
} Card_number;

struct Zone {
    size_t size;
    //Array of decks
    Deck **decks;
};

struct CardShape {
    char const *top, *middle1, *middle2, *bottom;
};

struct Card {
    Card_Type type;
    Card_number number;
    CardShape *cardShape;
    Card *next;
};

struct Deck {
    size_t size, capacity;
    Card *head, *tail;
};

/**
 * Get card number
 * @return
 */
char *getNumber(Card_number);

/**
 * Get card type
 * @return Card type string
 */
char *getType(Card_Type type);

/**
 * Create card
 * @param type
 * @param number
 * @return
 */
Card *createCard(Card_Type type, Card_number number);

/**
 * Create card shape depending on type and number
 * @param card
 */
void createCardShape(Card *card);

/**
 * Print card shape
 * @param card
 */
void printCardShape(Card *card);

/**
 * Print deck vertically
 * @param deck
 */
void printDeck(Deck *deck);

/**
 * Add card to deck
 * @param card Card to add
 * @param deck Deck to add the card to
 */
bool addCard(Card *card, Deck *deck);

/**
 * Create full deck of given cards capacity
 * @return Deck
 */
Deck *createDeck(size_t capacity);

/**
 * Fill deck with 52 cards
 * @param deck
 */
void fillInitialDeck(Deck *deck);

/**
 * Fill deck from initial deck
 * @param srcDeck deck to retrieve cards from
 * @param destDeck deck to move cards to
 * @param capacity capacity of new deck to fill
 */
void fillDeckFrom(Deck *srcDeck, Deck *destDeck);

/**
 * Shuffle deck cards order
 * @param deck Deck to shuffle
 * @param times number of shuffles
 */
void shuffleDeck(Deck *deck, size_t times);

/**
 * Get Card at position i
 * @param deck Deck to get the card from
 * @param i Card position
 * @return Card
 */
Card *cardAt(Deck *deck, size_t i);

/**
 * Move card at the tail from one deck to another
 * @param src Source Deck to move the card from
 * @param dest Destination Deck to move the card to
 */
void moveCard(Deck *src, Deck *dest);

/**
 * Test if we can place card1 on top of card2
 * @param card1
 * @param card2
 * @return 1 if compatible, else return 0
 */
bool isCompatible(Card *card1, Card *card2);

/**
 * Print card information as string
 * @param card
 */
void printCardInfo(Card *card);

Zone *createZone(size_t zone_size);

/**
 * Swap 2 cards in a linked list of card
 * @param head
 * @param a
 * @param b
 */
void swap(Card **head, Card *card1, Card *prevCard1, Card *card2, Card *prevCard2);

/**
 * Print zone 1
 * @param zone
 */
void printZone1(Zone *zone);

/**
 * Print zone 2 and 3 aligned vertically
 * @param zone
 */
void printZone23(Zone *zone2, Zone *zone3);

#endif //FREECELL_FREECELL_H
