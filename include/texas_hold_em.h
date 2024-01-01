#ifndef TEXASHOLDEM_H
#define TEXASHOLDEM_H

#include "card.h"

// Define constants for hand rankings
#define NOTHING 1
#define ONEPAIR 2
#define TWOPAIR 3
#define THREEOFAKIND 4
#define STRAIGHT 5
#define FLUSH 6
#define FULLHOUSE 7
#define FOUROFAKIND 8
#define STRAIGHTFLUSH 9
#define ROYALFLUSH 10

// Define a structure to store the result of a game
typedef struct {
    Hand hand;
    int win_condition;
} GameResult;

// Define a structure for the TexasHoldEm game
typedef struct {
    GameResult gameResult;
    Card gameSet[21][5];  // Stores possible combinations of hands and community cards
    Table table;          // Represents the community cards on the table
    Hand hand;            // Represents a player's hand
} TexasHoldEm;

// Initialize possible combinations of hand cards and community cards
void initialize_game_sets(TexasHoldEm* game);
int evaluate_hand(TexasHoldEm* game);
GameResult TexasHoldEm_play(TexasHoldEm* game);

#endif