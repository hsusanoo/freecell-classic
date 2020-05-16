//
// Created by haita on 2020/05/07.
//

#ifndef FREECELL_FREECELL_H
#define FREECELL_FREECELL_H

typedef struct Zone Zone;
typedef struct Deck Deck;
typedef struct Card Card;
typedef struct CardShape CardShape;

#define ZONE1_PADDING "\t\t"
#define ZONE1_SIZE 8
#define ZONE23_PADDING "\t"
#define ZONE23_SIZE 4
#define DECK_CAPACITY 52
#define SET_CAPACITY 13
#define CARD_TOP "╭────╮"
#define CARD_BOTTOM "╰────╯"
#define CARD_PLACEHOLDER (CardShape){"╭────╮", "│⢠⣒⡄ │", "│⠘⠭⠃ │", "╰────╯"}
#define CARD_PLACEHOLDER_EMPTY (CardShape){"╭────╮", "│    │", "│    │", "╰────╯"}
#define ZONE1_CONTROLS  "QSDFGHJK"
#define ZONE2_CONTROLS  "AZER"
#define CARD_SHAPES {"♣","♠","♦","♥"}

#include <stdlib.h>
#include "utils.h"

typedef enum {
    FAIL,
    SUCCESS,
    MOVE_ILLEGAL,
    DESTINATION_FULL,
    SOURCE_FULL
} Error;

typedef enum {
    Clubs,
    Spades,
    Diamonds,
    Hearts
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
 * Initialize game
 * @return 0 for losing game, 1 for winning game, 2 for new game, 3 for quitting
 */
int initGame();

/**
 * Prompt asking the user for new game
 * @return 1 for starting new game, else 0
 */
int askNewGame();

/**
 * Printing game won message
 */
void gameWonScreen();

/**
 * Printing game lost message
 */
void gameLostScreen();

/**
 * Move cards to zone 3 if possible
 * @param zone1
 * @param zone2
 * @param zone3
 * @return
 */
int autoCheckCards(Zone *zone1, Zone *zone2, Zone *zone3);

/**
 * Check if there are possible moves to do
 * @return
 */
int movePossible(Zone *zone1, Zone *zone2);

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
 * Get deck from zone by position
 * @param zone1
 * @param zone2
 * @param position
 * @return deck
 */
Deck *getDeck(Zone *zone1, Zone *zone2, char position);

/**
 * Get deck index from char
 * @param position
 * @return
 */
int getDeckIndex(char position);

/**
 * Shuffle deck cards order
 * @param deck Deck to shuffle
 * @param times number of shuffles
 */
void shuffleDeck(Deck *deck, size_t times);

/**
 * Get Card at position i
 * @param deck Deck to get the card from
 * @param index Card position
 * @return Card
 */
Card *cardAt(Deck *deck, size_t index);

/**
 * Move card at the tail from one deck to another
 * @param src Source Deck to move the card from
 * @param dest Destination Deck to move the card to
 * @return 1 if success, error code if not
 */
int moveCard(Deck *src, Deck *dest);

/**
 * Move card without checking rules
 * @param src
 * @param dest
 */
void plainMoveCard(Deck *src, Deck *dest);

/**
 * Test if we can place card1 on top of card2
 * @param card1
 * @param card2
 * @return 1 if compatible, else return 0
 */
bool isCompatible(Card *card1, Card *card2);

/**
 * Check if the deck's tail can be moved to zone 3
 * @param deck
 * @param zone3
 * @return
 */
bool isZone3Compatible(Deck *deck, Zone *zone3);

/**
 * Print card information as string
 * @param card
 */
void printCardInfo(Card *card);

/**
 * Create decks zone
 * @param zone_size
 * @return
 */
Zone *createZone(size_t zone_size);

/**
 * Swap 2 cards in a linked list of card
 * @param head
 * @param a
 * @param b
 */
void swap(Card **head, Card *card1, Card *prevCard1, Card *card2, Card *prevCard2);

/**
 * Fill zone 1 with cards shuffled
 * @param zone
 * @param shuffles number of shuffles
 */
void fillZone1(Zone *zone, size_t shuffles);

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

/**
 * Display all 3 zones
 * @param zone1
 * @param zone2
 * @param zone3
 */
void printLayout(Zone *zone1, Zone *zone2, Zone *zone3);

/**
 * Get top deck size from zone
 * @param zone
 */
size_t getTopSize(Zone *zone);

//TODO: check after each move if any card/s can be moved to zone 3

#endif //FREECELL_FREECELL_H
