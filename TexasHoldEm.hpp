#ifndef TEXASHOLDEM_HPP
#define TEXASHOLDEM_HPP

#include <cstdlib>
#include <random>

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
    Card cards[2];
} Hand;

typedef struct {
    Card cards[5];
} Table;

class TexasHoldEm {
private:
    std::mt19937 rng;
    Hand hand;
    Table table;

    Card deal_card() {
        std::uniform_int_distribution<int> suit_dist(1, 4);
        std::uniform_int_distribution<int> rank_dist(2, 14);
        return Card{suit_dist(rng), rank_dist(rng)};
    }

    bool evaluate_hand() {
        return false;
    }

public:
    TexasHoldEm();
    const Hand& get_hand() const;
    const Table& get_table() const;
    bool play();
};

#endif
