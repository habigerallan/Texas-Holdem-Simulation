#ifndef TEXASHOLDEM_HPP
#define TEXASHOLDEM_HPP

#include <cstdlib>
#include <random>
#include <unordered_map>
#include <algorithm>

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
#define HIGHCARD 1
#define ONEPAIR 2
#define TWOPAIR 3
#define THREEOFAKIND 4
#define STRAIGHT 5
#define FLUSH 6
#define FULLHOUSE 7
#define FOUROFAKIND 8
#define STRAIGHTFLUSH 9
#define ROYALFLUSH 10

typedef struct {
    int suit;
    int rank;
} Card;

struct Hand {
    Card cards[2];

	bool operator<(const Hand& other) const {
		if (cards[0].rank < other.cards[0].rank) {
			return true;
		} else if (cards[0].rank == other.cards[0].rank) {
			// If the ranks are equal, compare the second card's rank
			return cards[1].rank < other.cards[1].rank;
		}
		// If both ranks are equal, the hands are considered equal
		return false;
	}
};

typedef struct {
    Card cards[5];
} Table;

typedef struct {
    Hand hand;
	int win_condition;
} GameResult;

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

    int evaluate_hand() {
        Card combined[7];
		for (int i = 0; i < 2; i++) {
			combined[i] = hand.cards[i];
		}
		for (int i = 0; i < 5; i++) {
			combined[i + 2] = table.cards[i];
		}

		// Sort the cards by rank
		std::sort(combined, combined + 7, [](const Card &a, const Card &b) { return a.rank < b.rank; });

		std::unordered_map<int, int> rankCount, suitCount;
		int straightCount = 0, maxStraightCount = 0;
		int lastRank = -1;
		bool isFlush = false;

		for (int i = 0; i < 7; i++) {
			rankCount[combined[i].rank]++;
			suitCount[combined[i].suit]++;

			if (lastRank == combined[i].rank - 1) {
				straightCount++;
				maxStraightCount = std::max(maxStraightCount, straightCount);
			} else if (lastRank != combined[i].rank) {
				straightCount = 1;
			}
			lastRank = combined[i].rank;
		}

		for (auto &suit : suitCount) {
			if (suit.second >= 5) {
				isFlush = true;
				break;
			}
		}

		if (isFlush && maxStraightCount >= 5) {
			if (rankCount[TEN] && rankCount[JACK] && rankCount[QUEEN] && rankCount[KING] && rankCount[ACE]) {
				return ROYALFLUSH;
			}
			return STRAIGHTFLUSH;
		}

		int pairs = 0, threes = 0;
		for (auto &rank : rankCount) {
			if (rank.second == 4) {
				return FOUROFAKIND;
			} else if (rank.second == 3) {
				threes++;
			} else if (rank.second == 2) {
				pairs++;
			}
		}
		if (threes && pairs) {
			return FULLHOUSE;
		}
		if (isFlush) {
			return FLUSH;
		}
		if (maxStraightCount >= 5) {
			return STRAIGHT;
		}
		if (threes) {
			return THREEOFAKIND;
		}
		if (pairs >= 2) {
			return TWOPAIR;
		}
		if (pairs) {
			return ONEPAIR;
		}

		return HIGHCARD;
    }

public:
    TexasHoldEm();
    const Hand& get_hand() const;
    const Table& get_table() const;
    GameResult play();
};

#endif
