#ifndef TEXASHOLDEM_HPP
#define TEXASHOLDEM_HPP

#include <random>
#include "card.hpp"

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

// Define a structure for a player's hand
typedef struct {
    Card cards[2];
} Hand;

// Define a structure for the community cards on the table
typedef struct {
    Card cards[5];
} Table;

// Define a structure to store the result of a game
typedef struct {
    Hand hand;
	int win_condition;
} GameResult;

// Define the TexasHoldEm class
class TexasHoldEm {
	private:
		// Variables
        GameResult gameResult;
        Card gameSet[21][5];  // Stores possible combinations of hands and community cards
        std::mt19937 rng;  // Random number generator
        Table table;  // Represents the community cards on the table
        Hand hand;  // Represents a player's hand

        // Private helper functions
        inline Card deal_card();  // Deal a random card
        inline int evaluate_hand();  // Evaluate the player's hand
        inline void initialize_game_sets();  // Initialize the game sets

    public:
        // Constructor
        inline TexasHoldEm();

        // Public methods
        inline GameResult play();  // Play a round of Texas Hold'em
        inline const Hand& get_hand() const;  // Get the player's hand
        inline const Table& get_table() const;  // Get the community cards on the table
};


// Private Methods

// Deal a random card
inline Card TexasHoldEm::deal_card() {
    std::uniform_int_distribution<int> suit_dist(1, 4);
    std::uniform_int_distribution<int> rank_dist(2, 14);
    return Card{suit_dist(rng), rank_dist(rng)};
}

// Evaluate the player's hand
// FIXME basically logic is ass
inline int TexasHoldEm::evaluate_hand() {
	int highest = NOTHING;

	// Loop through all possible combinations of 5 cards (21 combinations).
	for (int i = 0; i < 21; ++i) {

		// Check for a Royal Flush (A, K, Q, J, 10 all of the same suit).
		if ((gameSet[i][0].suit == gameSet[i][1].suit && gameSet[i][1].suit == gameSet[i][2].suit && gameSet[i][2].suit == gameSet[i][3].suit && gameSet[i][3].suit == gameSet[i][4].suit) && 
			((gameSet[i][0].rank == TEN || gameSet[i][1].rank == TEN || gameSet[i][2].rank == TEN || gameSet[i][3].rank == TEN || gameSet[i][4].rank == TEN) &&
			(gameSet[i][0].rank == JACK || gameSet[i][1].rank == JACK || gameSet[i][2].rank == JACK || gameSet[i][3].rank == JACK || gameSet[i][4].rank == JACK) &&
			(gameSet[i][0].rank == QUEEN || gameSet[i][1].rank == QUEEN || gameSet[i][2].rank == QUEEN || gameSet[i][3].rank == QUEEN || gameSet[i][4].rank == QUEEN) &&
			(gameSet[i][0].rank == KING  || gameSet[i][1].rank == KING  || gameSet[i][2].rank == KING  || gameSet[i][3].rank == KING  || gameSet[i][4].rank == KING) &&
			(gameSet[i][0].rank == ACE || gameSet[i][1].rank == ACE || gameSet[i][2].rank == ACE || gameSet[i][3].rank == ACE || gameSet[i][4].rank == ACE))) {	
			return ROYALFLUSH;
		}

        // If we already have a higher rank of Straight Flush, continue to the next combination.
		if (highest == STRAIGHTFLUSH) {
			continue;
		}

        // Check for a Straight Flush (Five consecutive cards of the same suit).
		if ((gameSet[i][0].suit == gameSet[i][1].suit && gameSet[i][1].suit == gameSet[i][2].suit && gameSet[i][2].suit == gameSet[i][3].suit && gameSet[i][3].suit == gameSet[i][4].suit) &&
			(((gameSet[i][0].rank == TWO || gameSet[i][1].rank == TWO || gameSet[i][2].rank == TWO || gameSet[i][3].rank == TWO || gameSet[i][4].rank == TWO) &&
			(gameSet[i][0].rank == THREE || gameSet[i][1].rank == THREE || gameSet[i][2].rank == THREE || gameSet[i][3].rank == THREE || gameSet[i][4].rank == THREE) &&
			(gameSet[i][0].rank == FOUR || gameSet[i][1].rank == FOUR || gameSet[i][2].rank == FOUR || gameSet[i][3].rank == FOUR || gameSet[i][4].rank == FOUR) &&
			(gameSet[i][0].rank == FIVE  || gameSet[i][1].rank == FIVE  || gameSet[i][2].rank == FIVE  || gameSet[i][3].rank == FIVE  || gameSet[i][4].rank == FIVE) &&
			(gameSet[i][0].rank == SIX || gameSet[i][1].rank == SIX || gameSet[i][2].rank == SIX || gameSet[i][3].rank == SIX || gameSet[i][4].rank == SIX)) ||


			((gameSet[i][0].rank == THREE || gameSet[i][1].rank == THREE || gameSet[i][2].rank == THREE || gameSet[i][3].rank == THREE || gameSet[i][4].rank == THREE) &&
			(gameSet[i][0].rank == FOUR || gameSet[i][1].rank == FOUR || gameSet[i][2].rank == FOUR || gameSet[i][3].rank == FOUR || gameSet[i][4].rank == FOUR) &&
			(gameSet[i][0].rank == FIVE  || gameSet[i][1].rank == FIVE  || gameSet[i][2].rank == FIVE  || gameSet[i][3].rank == FIVE  || gameSet[i][4].rank == FIVE) &&
			(gameSet[i][0].rank == SIX || gameSet[i][1].rank == SIX || gameSet[i][2].rank == SIX || gameSet[i][3].rank == SIX || gameSet[i][4].rank == SIX) &&
			(gameSet[i][0].rank == SEVEN || gameSet[i][1].rank == SEVEN || gameSet[i][2].rank == SEVEN || gameSet[i][3].rank == SEVEN || gameSet[i][4].rank == SEVEN)) ||

			((gameSet[i][0].rank == FOUR || gameSet[i][1].rank == FOUR || gameSet[i][2].rank == FOUR || gameSet[i][3].rank == FOUR || gameSet[i][4].rank == FOUR) &&
			(gameSet[i][0].rank == FIVE  || gameSet[i][1].rank == FIVE  || gameSet[i][2].rank == FIVE  || gameSet[i][3].rank == FIVE  || gameSet[i][4].rank == FIVE) &&
			(gameSet[i][0].rank == SIX || gameSet[i][1].rank == SIX || gameSet[i][2].rank == SIX || gameSet[i][3].rank == SIX || gameSet[i][4].rank == SIX) &&
			(gameSet[i][0].rank == SEVEN || gameSet[i][1].rank == SEVEN || gameSet[i][2].rank == SEVEN || gameSet[i][3].rank == SEVEN || gameSet[i][4].rank == SEVEN) &&
			(gameSet[i][0].rank == EIGHT || gameSet[i][1].rank == EIGHT || gameSet[i][2].rank == EIGHT || gameSet[i][3].rank == EIGHT || gameSet[i][4].rank == EIGHT)) ||

			((gameSet[i][0].rank == FIVE  || gameSet[i][1].rank == FIVE  || gameSet[i][2].rank == FIVE  || gameSet[i][3].rank == FIVE  || gameSet[i][4].rank == FIVE) &&
			(gameSet[i][0].rank == SIX || gameSet[i][1].rank == SIX || gameSet[i][2].rank == SIX || gameSet[i][3].rank == SIX || gameSet[i][4].rank == SIX) &&
			(gameSet[i][0].rank == SEVEN || gameSet[i][1].rank == SEVEN || gameSet[i][2].rank == SEVEN || gameSet[i][3].rank == SEVEN || gameSet[i][4].rank == SEVEN) &&
			(gameSet[i][0].rank == EIGHT || gameSet[i][1].rank == EIGHT || gameSet[i][2].rank == EIGHT || gameSet[i][3].rank == EIGHT || gameSet[i][4].rank == EIGHT) &&
			(gameSet[i][0].rank == NINE || gameSet[i][1].rank == NINE || gameSet[i][2].rank == NINE || gameSet[i][3].rank == NINE || gameSet[i][4].rank == NINE)) ||

			((gameSet[i][0].rank == SIX || gameSet[i][1].rank == SIX || gameSet[i][2].rank == SIX || gameSet[i][3].rank == SIX || gameSet[i][4].rank == SIX) &&
			(gameSet[i][0].rank == SEVEN || gameSet[i][1].rank == SEVEN || gameSet[i][2].rank == SEVEN || gameSet[i][3].rank == SEVEN || gameSet[i][4].rank == SEVEN) &&
			(gameSet[i][0].rank == EIGHT || gameSet[i][1].rank == EIGHT || gameSet[i][2].rank == EIGHT || gameSet[i][3].rank == EIGHT || gameSet[i][4].rank == EIGHT) &&
			(gameSet[i][0].rank == NINE || gameSet[i][1].rank == NINE || gameSet[i][2].rank == NINE || gameSet[i][3].rank == NINE || gameSet[i][4].rank == NINE) &&
			(gameSet[i][0].rank == TEN || gameSet[i][1].rank == TEN || gameSet[i][2].rank == TEN || gameSet[i][3].rank == TEN || gameSet[i][4].rank == TEN)) ||

			((gameSet[i][0].rank == SEVEN || gameSet[i][1].rank == SEVEN || gameSet[i][2].rank == SEVEN || gameSet[i][3].rank == SEVEN || gameSet[i][4].rank == SEVEN) &&
			(gameSet[i][0].rank == EIGHT || gameSet[i][1].rank == EIGHT || gameSet[i][2].rank == EIGHT || gameSet[i][3].rank == EIGHT || gameSet[i][4].rank == EIGHT) &&
			(gameSet[i][0].rank == NINE || gameSet[i][1].rank == NINE || gameSet[i][2].rank == NINE || gameSet[i][3].rank == NINE || gameSet[i][4].rank == NINE) &&
			(gameSet[i][0].rank == TEN || gameSet[i][1].rank == TEN || gameSet[i][2].rank == TEN || gameSet[i][3].rank == TEN || gameSet[i][4].rank == TEN) &&
			(gameSet[i][0].rank == JACK || gameSet[i][1].rank == JACK || gameSet[i][2].rank == JACK || gameSet[i][3].rank == JACK || gameSet[i][4].rank == JACK)) ||

			((gameSet[i][0].rank == EIGHT || gameSet[i][1].rank == EIGHT || gameSet[i][2].rank == EIGHT || gameSet[i][3].rank == EIGHT || gameSet[i][4].rank == EIGHT) &&
			(gameSet[i][0].rank == NINE || gameSet[i][1].rank == NINE || gameSet[i][2].rank == NINE || gameSet[i][3].rank == NINE || gameSet[i][4].rank == NINE) &&
			(gameSet[i][0].rank == TEN || gameSet[i][1].rank == TEN || gameSet[i][2].rank == TEN || gameSet[i][3].rank == TEN || gameSet[i][4].rank == TEN) &&
			(gameSet[i][0].rank == JACK || gameSet[i][1].rank == JACK || gameSet[i][2].rank == JACK || gameSet[i][3].rank == JACK || gameSet[i][4].rank == JACK) &&
			(gameSet[i][0].rank == QUEEN || gameSet[i][1].rank == QUEEN || gameSet[i][2].rank == QUEEN || gameSet[i][3].rank == QUEEN || gameSet[i][4].rank == QUEEN)) ||

			((gameSet[i][0].rank == EIGHT || gameSet[i][1].rank == EIGHT || gameSet[i][2].rank == EIGHT || gameSet[i][3].rank == EIGHT || gameSet[i][4].rank == EIGHT) &&
			(gameSet[i][0].rank == NINE || gameSet[i][1].rank == NINE || gameSet[i][2].rank == NINE || gameSet[i][3].rank == NINE || gameSet[i][4].rank == NINE) &&
			(gameSet[i][0].rank == TEN || gameSet[i][1].rank == TEN || gameSet[i][2].rank == TEN || gameSet[i][3].rank == TEN || gameSet[i][4].rank == TEN) &&
			(gameSet[i][0].rank == JACK || gameSet[i][1].rank == JACK || gameSet[i][2].rank == JACK || gameSet[i][3].rank == JACK || gameSet[i][4].rank == JACK) &&
			(gameSet[i][0].rank == JACK || gameSet[i][1].rank == JACK || gameSet[i][2].rank == JACK || gameSet[i][3].rank == JACK || gameSet[i][4].rank == JACK) &&
			(gameSet[i][0].rank == KING || gameSet[i][1].rank == KING || gameSet[i][2].rank == KING || gameSet[i][3].rank == KING || gameSet[i][4].rank == KING)))) {
			highest = STRAIGHTFLUSH;
		}

        // If we already have a higher rank of Four of a Kind, continue to the next combination.
		if (highest == FOUROFAKIND) {
			continue;
		}

        // Check for Four of a Kind (Four cards with the same rank).
		if ((gameSet[i][0].rank == gameSet[i][1].rank && gameSet[i][1].rank == gameSet[i][2].rank && gameSet[i][2].rank == gameSet[i][3].rank) ||
			(gameSet[i][0].rank == gameSet[i][1].rank && gameSet[i][1].rank == gameSet[i][2].rank && gameSet[i][2].rank == gameSet[i][4].rank) ||
			(gameSet[i][0].rank == gameSet[i][1].rank && gameSet[i][1].rank == gameSet[i][3].rank && gameSet[i][3].rank == gameSet[i][4].rank) ||
			(gameSet[i][0].rank == gameSet[i][2].rank && gameSet[i][2].rank == gameSet[i][3].rank && gameSet[i][3].rank == gameSet[i][4].rank) ||
			(gameSet[i][1].rank == gameSet[i][2].rank && gameSet[i][2].rank == gameSet[i][3].rank && gameSet[i][3].rank == gameSet[i][4].rank)) {
			highest = FOUROFAKIND;
		}

        // If we already have a higher rank of Full House, continue to the next combination.
		if (highest == FULLHOUSE) {
			continue;
		}

        // Check for a Full House (Three cards of one rank and two cards of another rank).
		if ((gameSet[i][0].rank == gameSet[i][1].rank && gameSet[i][1].rank == gameSet[i][2].rank && gameSet[i][3].rank == gameSet[i][4].rank) ||
			(gameSet[i][0].rank == gameSet[i][1].rank && gameSet[i][1].rank == gameSet[i][3].rank && gameSet[i][2].rank == gameSet[i][4].rank) ||
			(gameSet[i][0].rank == gameSet[i][1].rank && gameSet[i][1].rank == gameSet[i][4].rank && gameSet[i][2].rank == gameSet[i][3].rank) ||
			(gameSet[i][0].rank == gameSet[i][2].rank && gameSet[i][2].rank == gameSet[i][3].rank && gameSet[i][1].rank == gameSet[i][4].rank) ||
			(gameSet[i][0].rank == gameSet[i][2].rank && gameSet[i][2].rank == gameSet[i][4].rank && gameSet[i][1].rank == gameSet[i][3].rank) ||
			(gameSet[i][0].rank == gameSet[i][3].rank && gameSet[i][3].rank == gameSet[i][4].rank && gameSet[i][1].rank == gameSet[i][2].rank) ||
			(gameSet[i][1].rank == gameSet[i][2].rank && gameSet[i][2].rank == gameSet[i][3].rank && gameSet[i][0].rank == gameSet[i][4].rank) ||
			(gameSet[i][1].rank == gameSet[i][2].rank && gameSet[i][2].rank == gameSet[i][4].rank && gameSet[i][0].rank == gameSet[i][3].rank) ||
			(gameSet[i][1].rank == gameSet[i][3].rank && gameSet[i][3].rank == gameSet[i][4].rank && gameSet[i][0].rank == gameSet[i][2].rank) ||
			(gameSet[i][2].rank == gameSet[i][3].rank && gameSet[i][3].rank == gameSet[i][4].rank && gameSet[i][0].rank == gameSet[i][1].rank)) {
			highest = FULLHOUSE;
		}

        // If we already have a higher rank of Flush, continue to the next combination.
		if (highest == FLUSH) {
			continue;
		}

        // Check for a Flush (All cards of the same suit).
		if (gameSet[i][0].suit == gameSet[i][1].suit && gameSet[i][1].suit == gameSet[i][2].suit && gameSet[i][2].suit == gameSet[i][3].suit && gameSet[i][3].suit == gameSet[i][4].suit) {
			highest = FLUSH;
		}

        // If we already have a higher rank of Straight, continue to the next combination.
		if (highest == STRAIGHT) {
			continue;
		}

        // Check for a Straight (Five consecutive cards of any suit).
		if ((gameSet[i][0].rank == TWO && gameSet[i][1].rank == THREE && gameSet[i][2].rank == FOUR && gameSet[i][3].rank == FIVE && gameSet[i][4].rank == SIX) ||
			(gameSet[i][0].rank == THREE && gameSet[i][1].rank == FOUR && gameSet[i][2].rank == FIVE && gameSet[i][3].rank == SIX && gameSet[i][4].rank == SEVEN) ||
			(gameSet[i][0].rank == FOUR && gameSet[i][1].rank == FIVE && gameSet[i][2].rank == SIX && gameSet[i][3].rank == SEVEN && gameSet[i][4].rank == EIGHT) ||
			(gameSet[i][0].rank == FIVE && gameSet[i][1].rank == SIX && gameSet[i][2].rank == SEVEN && gameSet[i][3].rank == EIGHT && gameSet[i][4].rank == NINE) ||
			(gameSet[i][0].rank == SIX && gameSet[i][1].rank == SEVEN && gameSet[i][2].rank == EIGHT && gameSet[i][3].rank == NINE && gameSet[i][4].rank == TEN) ||
			(gameSet[i][0].rank == SEVEN && gameSet[i][1].rank == EIGHT && gameSet[i][2].rank == NINE && gameSet[i][3].rank == TEN && gameSet[i][4].rank == JACK) ||
			(gameSet[i][0].rank == EIGHT && gameSet[i][1].rank == NINE && gameSet[i][2].rank == TEN && gameSet[i][3].rank == JACK && gameSet[i][4].rank == QUEEN) ||
			(gameSet[i][0].rank == NINE && gameSet[i][1].rank == TEN && gameSet[i][2].rank == JACK && gameSet[i][3].rank == QUEEN && gameSet[i][4].rank == KING)) {
			highest = STRAIGHT;
		}

        // If we already have a higher rank of Three of a Kind, continue to the next combination.
		if (highest == THREEOFAKIND) {
			continue;
		}

        // Check for Three of a Kind (Three cards with the same rank).
		if ((gameSet[i][0].rank == gameSet[i][1].rank && gameSet[i][1].rank == gameSet[i][2].rank) ||
			(gameSet[i][0].rank == gameSet[i][1].rank && gameSet[i][1].rank == gameSet[i][3].rank) ||
			(gameSet[i][0].rank == gameSet[i][1].rank && gameSet[i][1].rank == gameSet[i][4].rank) ||
			(gameSet[i][0].rank == gameSet[i][2].rank && gameSet[i][2].rank == gameSet[i][3].rank) ||
			(gameSet[i][0].rank == gameSet[i][2].rank && gameSet[i][2].rank == gameSet[i][4].rank) ||
			(gameSet[i][0].rank == gameSet[i][3].rank && gameSet[i][3].rank == gameSet[i][4].rank) ||
			(gameSet[i][1].rank == gameSet[i][2].rank && gameSet[i][2].rank == gameSet[i][3].rank) ||
			(gameSet[i][1].rank == gameSet[i][2].rank && gameSet[i][2].rank == gameSet[i][4].rank) ||
			(gameSet[i][1].rank == gameSet[i][3].rank && gameSet[i][3].rank == gameSet[i][4].rank) ||
			(gameSet[i][2].rank == gameSet[i][3].rank && gameSet[i][3].rank == gameSet[i][4].rank)) {
			highest = THREEOFAKIND;
		}

        // If we already have a higher rank of Two Pair, continue to the next combination.
		if (highest == TWOPAIR) {
			continue;
		}

        // Check for Two Pair (Two cards of one rank and two cards of another rank).
		if ((gameSet[i][0].rank == gameSet[i][1].rank && gameSet[i][2].rank == gameSet[i][3].rank) ||
			(gameSet[i][0].rank == gameSet[i][1].rank && gameSet[i][2].rank == gameSet[i][4].rank) ||
			(gameSet[i][0].rank == gameSet[i][1].rank && gameSet[i][3].rank == gameSet[i][4].rank) ||

			(gameSet[i][0].rank == gameSet[i][2].rank && gameSet[i][1].rank == gameSet[i][3].rank) ||
			(gameSet[i][0].rank == gameSet[i][2].rank && gameSet[i][1].rank == gameSet[i][4].rank) ||
			(gameSet[i][0].rank == gameSet[i][2].rank && gameSet[i][3].rank == gameSet[i][4].rank) ||

			(gameSet[i][0].rank == gameSet[i][3].rank && gameSet[i][1].rank == gameSet[i][2].rank) ||
			(gameSet[i][0].rank == gameSet[i][3].rank && gameSet[i][1].rank == gameSet[i][4].rank) ||
			(gameSet[i][0].rank == gameSet[i][3].rank && gameSet[i][2].rank == gameSet[i][4].rank) ||

			(gameSet[i][0].rank == gameSet[i][4].rank && gameSet[i][1].rank == gameSet[i][2].rank) ||
			(gameSet[i][0].rank == gameSet[i][4].rank && gameSet[i][1].rank == gameSet[i][3].rank) ||
			(gameSet[i][0].rank == gameSet[i][4].rank && gameSet[i][2].rank == gameSet[i][3].rank) ||

			(gameSet[i][1].rank == gameSet[i][2].rank && gameSet[i][3].rank == gameSet[i][4].rank) ||
			(gameSet[i][1].rank == gameSet[i][3].rank && gameSet[i][2].rank == gameSet[i][4].rank) ||
			(gameSet[i][1].rank == gameSet[i][4].rank && gameSet[i][2].rank == gameSet[i][3].rank)) {
			highest = TWOPAIR;
		}

        // If we already have a higher rank of One Pair, continue to the next combination.
		if (highest == ONEPAIR) {
			continue;
		}

        // Check for One Pair (Two cards with the same rank).
		if ((gameSet[i][0].rank == gameSet[i][1].rank) ||
			(gameSet[i][0].rank == gameSet[i][2].rank) ||
			(gameSet[i][0].rank == gameSet[i][3].rank) ||
			(gameSet[i][0].rank == gameSet[i][4].rank) ||
			
			(gameSet[i][1].rank == gameSet[i][2].rank) ||
			(gameSet[i][1].rank == gameSet[i][3].rank) ||
			(gameSet[i][1].rank == gameSet[i][4].rank) ||
			
			(gameSet[i][2].rank == gameSet[i][3].rank) ||
			(gameSet[i][2].rank == gameSet[i][4].rank) ||
			
			(gameSet[i][3].rank == gameSet[i][4].rank)) {
			highest = ONEPAIR;
		}
	}

    // Return the highest rank found in the combinations.
	return highest;
}

// Initialize possible combinations of hand cards and community cards
inline void TexasHoldEm::initialize_game_sets() {
    Card hc1 = hand.cards[0];
    Card hc2 = hand.cards[1];

    Card tc1 = table.cards[0];
    Card tc2 = table.cards[1];
    Card tc3 = table.cards[2];
    Card tc4 = table.cards[3];
    Card tc5 = table.cards[4];

 	// Populate the gameSet array with all possible combinations of cards.
    // Each row in gameSet represents a unique combination of cards to evaluate.
    gameSet[0][0] = hc1; gameSet[0][1] = hc2; gameSet[0][2] = tc1; gameSet[0][3] = tc2; gameSet[0][4] = tc3;
	gameSet[1][0] = hc1; gameSet[1][1] = hc2; gameSet[1][2] = tc1; gameSet[1][3] = tc2; gameSet[1][4] = tc4;
	gameSet[2][0] = hc1; gameSet[2][1] = hc2; gameSet[2][2] = tc1; gameSet[2][3] = tc2; gameSet[2][4] = tc5;
	gameSet[3][0] = hc1; gameSet[3][1] = hc2; gameSet[3][2] = tc1; gameSet[3][3] = tc3; gameSet[3][4] = tc4;
	gameSet[4][0] = hc1; gameSet[4][1] = hc2; gameSet[4][2] = tc1; gameSet[4][3] = tc3; gameSet[4][4] = tc5;
	gameSet[5][0] = hc1; gameSet[5][1] = hc2; gameSet[5][2] = tc1; gameSet[5][3] = tc4; gameSet[5][4] = tc5;
	gameSet[6][0] = hc1; gameSet[6][1] = hc2; gameSet[6][2] = tc2; gameSet[6][3] = tc3; gameSet[6][4] = tc4;
	gameSet[7][0] = hc1; gameSet[7][1] = hc2; gameSet[7][2] = tc2; gameSet[7][3] = tc3; gameSet[7][4] = tc5;
	gameSet[8][0] = hc1; gameSet[8][1] = hc2; gameSet[8][2] = tc2; gameSet[8][3] = tc4; gameSet[8][4] = tc5;
	gameSet[9][0] = hc1; gameSet[9][1] = hc2; gameSet[9][2] = tc3; gameSet[9][3] = tc4; gameSet[9][4] = tc5;
	gameSet[10][0] = hc1; gameSet[10][1] = tc1; gameSet[10][2] = tc2; gameSet[10][3] = tc3; gameSet[10][4] = tc4;
	gameSet[11][0] = hc1; gameSet[11][1] = tc1; gameSet[11][2] = tc2; gameSet[11][3] = tc3; gameSet[11][4] = tc5;
	gameSet[12][0] = hc1; gameSet[12][1] = tc1; gameSet[12][2] = tc2; gameSet[12][3] = tc4; gameSet[12][4] = tc5;
	gameSet[13][0] = hc1; gameSet[13][1] = tc1; gameSet[13][2] = tc3; gameSet[13][3] = tc4; gameSet[13][4] = tc5;
	gameSet[14][0] = hc1; gameSet[14][1] = tc2; gameSet[14][2] = tc3; gameSet[14][3] = tc4; gameSet[14][4] = tc5;
	gameSet[15][0] = hc2; gameSet[15][1] = tc1; gameSet[15][2] = tc2; gameSet[15][3] = tc3; gameSet[15][4] = tc4;
	gameSet[16][0] = hc2; gameSet[16][1] = tc1; gameSet[16][2] = tc2; gameSet[16][3] = tc3; gameSet[16][4] = tc5;
	gameSet[17][0] = hc2; gameSet[17][1] = tc1; gameSet[17][2] = tc2; gameSet[17][3] = tc4; gameSet[17][4] = tc5;
	gameSet[18][0] = hc2; gameSet[18][1] = tc1; gameSet[18][2] = tc3; gameSet[18][3] = tc4; gameSet[18][4] = tc5;
	gameSet[19][0] = hc2; gameSet[19][1] = tc2; gameSet[19][2] = tc3; gameSet[19][3] = tc4; gameSet[19][4] = tc5;
	gameSet[20][0] = tc1; gameSet[20][1] = tc2; gameSet[20][2] = tc3; gameSet[20][3] = tc4; gameSet[20][4] = tc5;
}


// Public Methods

// Constructor
inline TexasHoldEm::TexasHoldEm() : rng(std::random_device()()) {}

// Play a round of Texas Hold'em
inline GameResult TexasHoldEm::play() {
	hand.cards[0] = deal_card();
	hand.cards[1] = deal_card();

	table.cards[0] = deal_card();
	table.cards[1] = deal_card();
	table.cards[2] = deal_card();
	table.cards[3] = deal_card();
	table.cards[4] = deal_card();

	initialize_game_sets();

	gameResult.hand = hand;
	gameResult.win_condition = evaluate_hand();

	return gameResult;
}

// Get the player's hand
inline const Hand& TexasHoldEm::get_hand() const {
    return hand;
}

// Get the community cards on the table
inline const Table& TexasHoldEm::get_table() const {
    return table;
}

#endif
