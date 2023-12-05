#ifndef POKER_H
#define POKER_H

#include <stdio.h>
#include <stdlib.h>

#define HEARTS 1
#define DIAMONDS 2
#define CLUBS 3
#define SPADES 4
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

typedef struct {
    int suit;
    int rank;
} Card;

typedef struct {
    Card *cards;
    int checks;
} Hand;

typedef struct {
    Hand *hand
} Player;

typedef struct {
    Card flop[3];
    Card turn;
    Card river;
} Game;

Card deal_card();
void *play_chunk(void *arg);
int evaluate_hand(Player *players, Game *game);

#endif
