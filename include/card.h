#ifndef CARD_H
#define CARD_H

#include <stdlib.h>
#include <time.h>

// Define constants for card suits
#define HEARTS 1
#define DIAMONDS 2
#define CLUBS 3
#define SPADES 4

// Define constants for card ranks
#define TWO 2
#define THREE 3
#define FOUR 4
#define FIVE 5
#define SIX 6
#define SEVEN 7
#define EIGHT 8
#define NINE 9
#define TEN 10
#define JACK 11
#define QUEEN 12
#define KING 13
#define ACE 14

// Define a Card structure to represent a playing card
typedef struct {
    int suit;
    int rank;
} Card;

void initialize_random();
Card deal_card();

#endif