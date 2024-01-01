#include "texas_hold_em.h"

void initialize_game_sets(TexasHoldEm* game) {
    Card hc1 = game->hand.cards[0];
    Card hc2 = game->hand.cards[1];

    Card tc1 = game->table.cards[0];
    Card tc2 = game->table.cards[1];
    Card tc3 = game->table.cards[2];
    Card tc4 = game->table.cards[3];
    Card tc5 = game->table.cards[4];

 	// Populate the gameSet array with all possible combinations of cards.
    // Each row in gameSet represents a unique combination of cards to evaluate.
    game->gameSet[0][0] = hc1; game->gameSet[0][1] = hc2; game->gameSet[0][2] = tc1; game->gameSet[0][3] = tc2; game->gameSet[0][4] = tc3;
	game->gameSet[1][0] = hc1; game->gameSet[1][1] = hc2; game->gameSet[1][2] = tc1; game->gameSet[1][3] = tc2; game->gameSet[1][4] = tc4;
	game->gameSet[2][0] = hc1; game->gameSet[2][1] = hc2; game->gameSet[2][2] = tc1; game->gameSet[2][3] = tc2; game->gameSet[2][4] = tc5;
	game->gameSet[3][0] = hc1; game->gameSet[3][1] = hc2; game->gameSet[3][2] = tc1; game->gameSet[3][3] = tc3; game->gameSet[3][4] = tc4;
	game->gameSet[4][0] = hc1; game->gameSet[4][1] = hc2; game->gameSet[4][2] = tc1; game->gameSet[4][3] = tc3; game->gameSet[4][4] = tc5;
	game->gameSet[5][0] = hc1; game->gameSet[5][1] = hc2; game->gameSet[5][2] = tc1; game->gameSet[5][3] = tc4; game->gameSet[5][4] = tc5;
	game->gameSet[6][0] = hc1; game->gameSet[6][1] = hc2; game->gameSet[6][2] = tc2; game->gameSet[6][3] = tc3; game->gameSet[6][4] = tc4;
	game->gameSet[7][0] = hc1; game->gameSet[7][1] = hc2; game->gameSet[7][2] = tc2; game->gameSet[7][3] = tc3; game->gameSet[7][4] = tc5;
	game->gameSet[8][0] = hc1; game->gameSet[8][1] = hc2; game->gameSet[8][2] = tc2; game->gameSet[8][3] = tc4; game->gameSet[8][4] = tc5;
	game->gameSet[9][0] = hc1; game->gameSet[9][1] = hc2; game->gameSet[9][2] = tc3; game->gameSet[9][3] = tc4; game->gameSet[9][4] = tc5;
	game->gameSet[10][0] = hc1; game->gameSet[10][1] = tc1; game->gameSet[10][2] = tc2; game->gameSet[10][3] = tc3; game->gameSet[10][4] = tc4;
	game->gameSet[11][0] = hc1; game->gameSet[11][1] = tc1; game->gameSet[11][2] = tc2; game->gameSet[11][3] = tc3; game->gameSet[11][4] = tc5;
	game->gameSet[12][0] = hc1; game->gameSet[12][1] = tc1; game->gameSet[12][2] = tc2; game->gameSet[12][3] = tc4; game->gameSet[12][4] = tc5;
	game->gameSet[13][0] = hc1; game->gameSet[13][1] = tc1; game->gameSet[13][2] = tc3; game->gameSet[13][3] = tc4; game->gameSet[13][4] = tc5;
	game->gameSet[14][0] = hc1; game->gameSet[14][1] = tc2; game->gameSet[14][2] = tc3; game->gameSet[14][3] = tc4; game->gameSet[14][4] = tc5;
	game->gameSet[15][0] = hc2; game->gameSet[15][1] = tc1; game->gameSet[15][2] = tc2; game->gameSet[15][3] = tc3; game->gameSet[15][4] = tc4;
	game->gameSet[16][0] = hc2; game->gameSet[16][1] = tc1; game->gameSet[16][2] = tc2; game->gameSet[16][3] = tc3; game->gameSet[16][4] = tc5;
	game->gameSet[17][0] = hc2; game->gameSet[17][1] = tc1; game->gameSet[17][2] = tc2; game->gameSet[17][3] = tc4; game->gameSet[17][4] = tc5;
	game->gameSet[18][0] = hc2; game->gameSet[18][1] = tc1; game->gameSet[18][2] = tc3; game->gameSet[18][3] = tc4; game->gameSet[18][4] = tc5;
	game->gameSet[19][0] = hc2; game->gameSet[19][1] = tc2; game->gameSet[19][2] = tc3; game->gameSet[19][3] = tc4; game->gameSet[19][4] = tc5;
	game->gameSet[20][0] = tc1; game->gameSet[20][1] = tc2; game->gameSet[20][2] = tc3; game->gameSet[20][3] = tc4; game->gameSet[20][4] = tc5;
}

int evaluate_hand(TexasHoldEm* game) {
    int highest = NOTHING;

	// Loop through all possible combinations of 5 cards (21 combinations).
	for (int i = 0; i < 21; ++i) {

		// Check for a Royal Flush (A, K, Q, J, 10 all of the same suit).
		if ((game->gameSet[i][0].suit == game->gameSet[i][1].suit && game->gameSet[i][1].suit == game->gameSet[i][2].suit && game->gameSet[i][2].suit == game->gameSet[i][3].suit && game->gameSet[i][3].suit == game->gameSet[i][4].suit) && 
			((game->gameSet[i][0].rank == TEN || game->gameSet[i][1].rank == TEN || game->gameSet[i][2].rank == TEN || game->gameSet[i][3].rank == TEN || game->gameSet[i][4].rank == TEN) &&
			(game->gameSet[i][0].rank == JACK || game->gameSet[i][1].rank == JACK || game->gameSet[i][2].rank == JACK || game->gameSet[i][3].rank == JACK || game->gameSet[i][4].rank == JACK) &&
			(game->gameSet[i][0].rank == QUEEN || game->gameSet[i][1].rank == QUEEN || game->gameSet[i][2].rank == QUEEN || game->gameSet[i][3].rank == QUEEN || game->gameSet[i][4].rank == QUEEN) &&
			(game->gameSet[i][0].rank == KING  || game->gameSet[i][1].rank == KING  || game->gameSet[i][2].rank == KING  || game->gameSet[i][3].rank == KING  || game->gameSet[i][4].rank == KING) &&
			(game->gameSet[i][0].rank == ACE || game->gameSet[i][1].rank == ACE || game->gameSet[i][2].rank == ACE || game->gameSet[i][3].rank == ACE || game->gameSet[i][4].rank == ACE))) {	
			return ROYALFLUSH;
		}

        // If we already have a higher rank of Straight Flush, continue to the next combination.
		if (highest == STRAIGHTFLUSH) {
			continue;
		}

        // Check for a Straight Flush (Five consecutive cards of the same suit).
		if ((game->gameSet[i][0].suit == game->gameSet[i][1].suit && game->gameSet[i][1].suit == game->gameSet[i][2].suit && game->gameSet[i][2].suit == game->gameSet[i][3].suit && game->gameSet[i][3].suit == game->gameSet[i][4].suit) &&
			(((game->gameSet[i][0].rank == TWO || game->gameSet[i][1].rank == TWO || game->gameSet[i][2].rank == TWO || game->gameSet[i][3].rank == TWO || game->gameSet[i][4].rank == TWO) &&
			(game->gameSet[i][0].rank == THREE || game->gameSet[i][1].rank == THREE || game->gameSet[i][2].rank == THREE || game->gameSet[i][3].rank == THREE || game->gameSet[i][4].rank == THREE) &&
			(game->gameSet[i][0].rank == FOUR || game->gameSet[i][1].rank == FOUR || game->gameSet[i][2].rank == FOUR || game->gameSet[i][3].rank == FOUR || game->gameSet[i][4].rank == FOUR) &&
			(game->gameSet[i][0].rank == FIVE  || game->gameSet[i][1].rank == FIVE  || game->gameSet[i][2].rank == FIVE  || game->gameSet[i][3].rank == FIVE  || game->gameSet[i][4].rank == FIVE) &&
			(game->gameSet[i][0].rank == SIX || game->gameSet[i][1].rank == SIX || game->gameSet[i][2].rank == SIX || game->gameSet[i][3].rank == SIX || game->gameSet[i][4].rank == SIX)) ||


			((game->gameSet[i][0].rank == THREE || game->gameSet[i][1].rank == THREE || game->gameSet[i][2].rank == THREE || game->gameSet[i][3].rank == THREE || game->gameSet[i][4].rank == THREE) &&
			(game->gameSet[i][0].rank == FOUR || game->gameSet[i][1].rank == FOUR || game->gameSet[i][2].rank == FOUR || game->gameSet[i][3].rank == FOUR || game->gameSet[i][4].rank == FOUR) &&
			(game->gameSet[i][0].rank == FIVE  || game->gameSet[i][1].rank == FIVE  || game->gameSet[i][2].rank == FIVE  || game->gameSet[i][3].rank == FIVE  || game->gameSet[i][4].rank == FIVE) &&
			(game->gameSet[i][0].rank == SIX || game->gameSet[i][1].rank == SIX || game->gameSet[i][2].rank == SIX || game->gameSet[i][3].rank == SIX || game->gameSet[i][4].rank == SIX) &&
			(game->gameSet[i][0].rank == SEVEN || game->gameSet[i][1].rank == SEVEN || game->gameSet[i][2].rank == SEVEN || game->gameSet[i][3].rank == SEVEN || game->gameSet[i][4].rank == SEVEN)) ||

			((game->gameSet[i][0].rank == FOUR || game->gameSet[i][1].rank == FOUR || game->gameSet[i][2].rank == FOUR || game->gameSet[i][3].rank == FOUR || game->gameSet[i][4].rank == FOUR) &&
			(game->gameSet[i][0].rank == FIVE  || game->gameSet[i][1].rank == FIVE  || game->gameSet[i][2].rank == FIVE  || game->gameSet[i][3].rank == FIVE  || game->gameSet[i][4].rank == FIVE) &&
			(game->gameSet[i][0].rank == SIX || game->gameSet[i][1].rank == SIX || game->gameSet[i][2].rank == SIX || game->gameSet[i][3].rank == SIX || game->gameSet[i][4].rank == SIX) &&
			(game->gameSet[i][0].rank == SEVEN || game->gameSet[i][1].rank == SEVEN || game->gameSet[i][2].rank == SEVEN || game->gameSet[i][3].rank == SEVEN || game->gameSet[i][4].rank == SEVEN) &&
			(game->gameSet[i][0].rank == EIGHT || game->gameSet[i][1].rank == EIGHT || game->gameSet[i][2].rank == EIGHT || game->gameSet[i][3].rank == EIGHT || game->gameSet[i][4].rank == EIGHT)) ||

			((game->gameSet[i][0].rank == FIVE  || game->gameSet[i][1].rank == FIVE  || game->gameSet[i][2].rank == FIVE  || game->gameSet[i][3].rank == FIVE  || game->gameSet[i][4].rank == FIVE) &&
			(game->gameSet[i][0].rank == SIX || game->gameSet[i][1].rank == SIX || game->gameSet[i][2].rank == SIX || game->gameSet[i][3].rank == SIX || game->gameSet[i][4].rank == SIX) &&
			(game->gameSet[i][0].rank == SEVEN || game->gameSet[i][1].rank == SEVEN || game->gameSet[i][2].rank == SEVEN || game->gameSet[i][3].rank == SEVEN || game->gameSet[i][4].rank == SEVEN) &&
			(game->gameSet[i][0].rank == EIGHT || game->gameSet[i][1].rank == EIGHT || game->gameSet[i][2].rank == EIGHT || game->gameSet[i][3].rank == EIGHT || game->gameSet[i][4].rank == EIGHT) &&
			(game->gameSet[i][0].rank == NINE || game->gameSet[i][1].rank == NINE || game->gameSet[i][2].rank == NINE || game->gameSet[i][3].rank == NINE || game->gameSet[i][4].rank == NINE)) ||

			((game->gameSet[i][0].rank == SIX || game->gameSet[i][1].rank == SIX || game->gameSet[i][2].rank == SIX || game->gameSet[i][3].rank == SIX || game->gameSet[i][4].rank == SIX) &&
			(game->gameSet[i][0].rank == SEVEN || game->gameSet[i][1].rank == SEVEN || game->gameSet[i][2].rank == SEVEN || game->gameSet[i][3].rank == SEVEN || game->gameSet[i][4].rank == SEVEN) &&
			(game->gameSet[i][0].rank == EIGHT || game->gameSet[i][1].rank == EIGHT || game->gameSet[i][2].rank == EIGHT || game->gameSet[i][3].rank == EIGHT || game->gameSet[i][4].rank == EIGHT) &&
			(game->gameSet[i][0].rank == NINE || game->gameSet[i][1].rank == NINE || game->gameSet[i][2].rank == NINE || game->gameSet[i][3].rank == NINE || game->gameSet[i][4].rank == NINE) &&
			(game->gameSet[i][0].rank == TEN || game->gameSet[i][1].rank == TEN || game->gameSet[i][2].rank == TEN || game->gameSet[i][3].rank == TEN || game->gameSet[i][4].rank == TEN)) ||

			((game->gameSet[i][0].rank == SEVEN || game->gameSet[i][1].rank == SEVEN || game->gameSet[i][2].rank == SEVEN || game->gameSet[i][3].rank == SEVEN || game->gameSet[i][4].rank == SEVEN) &&
			(game->gameSet[i][0].rank == EIGHT || game->gameSet[i][1].rank == EIGHT || game->gameSet[i][2].rank == EIGHT || game->gameSet[i][3].rank == EIGHT || game->gameSet[i][4].rank == EIGHT) &&
			(game->gameSet[i][0].rank == NINE || game->gameSet[i][1].rank == NINE || game->gameSet[i][2].rank == NINE || game->gameSet[i][3].rank == NINE || game->gameSet[i][4].rank == NINE) &&
			(game->gameSet[i][0].rank == TEN || game->gameSet[i][1].rank == TEN || game->gameSet[i][2].rank == TEN || game->gameSet[i][3].rank == TEN || game->gameSet[i][4].rank == TEN) &&
			(game->gameSet[i][0].rank == JACK || game->gameSet[i][1].rank == JACK || game->gameSet[i][2].rank == JACK || game->gameSet[i][3].rank == JACK || game->gameSet[i][4].rank == JACK)) ||

			((game->gameSet[i][0].rank == EIGHT || game->gameSet[i][1].rank == EIGHT || game->gameSet[i][2].rank == EIGHT || game->gameSet[i][3].rank == EIGHT || game->gameSet[i][4].rank == EIGHT) &&
			(game->gameSet[i][0].rank == NINE || game->gameSet[i][1].rank == NINE || game->gameSet[i][2].rank == NINE || game->gameSet[i][3].rank == NINE || game->gameSet[i][4].rank == NINE) &&
			(game->gameSet[i][0].rank == TEN || game->gameSet[i][1].rank == TEN || game->gameSet[i][2].rank == TEN || game->gameSet[i][3].rank == TEN || game->gameSet[i][4].rank == TEN) &&
			(game->gameSet[i][0].rank == JACK || game->gameSet[i][1].rank == JACK || game->gameSet[i][2].rank == JACK || game->gameSet[i][3].rank == JACK || game->gameSet[i][4].rank == JACK) &&
			(game->gameSet[i][0].rank == QUEEN || game->gameSet[i][1].rank == QUEEN || game->gameSet[i][2].rank == QUEEN || game->gameSet[i][3].rank == QUEEN || game->gameSet[i][4].rank == QUEEN)) ||

			((game->gameSet[i][0].rank == EIGHT || game->gameSet[i][1].rank == EIGHT || game->gameSet[i][2].rank == EIGHT || game->gameSet[i][3].rank == EIGHT || game->gameSet[i][4].rank == EIGHT) &&
			(game->gameSet[i][0].rank == NINE || game->gameSet[i][1].rank == NINE || game->gameSet[i][2].rank == NINE || game->gameSet[i][3].rank == NINE || game->gameSet[i][4].rank == NINE) &&
			(game->gameSet[i][0].rank == TEN || game->gameSet[i][1].rank == TEN || game->gameSet[i][2].rank == TEN || game->gameSet[i][3].rank == TEN || game->gameSet[i][4].rank == TEN) &&
			(game->gameSet[i][0].rank == JACK || game->gameSet[i][1].rank == JACK || game->gameSet[i][2].rank == JACK || game->gameSet[i][3].rank == JACK || game->gameSet[i][4].rank == JACK) &&
			(game->gameSet[i][0].rank == JACK || game->gameSet[i][1].rank == JACK || game->gameSet[i][2].rank == JACK || game->gameSet[i][3].rank == JACK || game->gameSet[i][4].rank == JACK) &&
			(game->gameSet[i][0].rank == KING || game->gameSet[i][1].rank == KING || game->gameSet[i][2].rank == KING || game->gameSet[i][3].rank == KING || game->gameSet[i][4].rank == KING)))) {
			highest = STRAIGHTFLUSH;
			continue;
		}

        // If we already have a higher rank of Four of a Kind, continue to the next combination.
		if (highest == FOUROFAKIND) {
			continue;
		}

        // Check for Four of a Kind (Four cards with the same rank).
		if ((game->gameSet[i][0].rank == game->gameSet[i][1].rank && game->gameSet[i][1].rank == game->gameSet[i][2].rank && game->gameSet[i][2].rank == game->gameSet[i][3].rank) ||
			(game->gameSet[i][0].rank == game->gameSet[i][1].rank && game->gameSet[i][1].rank == game->gameSet[i][2].rank && game->gameSet[i][2].rank == game->gameSet[i][4].rank) ||
			(game->gameSet[i][0].rank == game->gameSet[i][1].rank && game->gameSet[i][1].rank == game->gameSet[i][3].rank && game->gameSet[i][3].rank == game->gameSet[i][4].rank) ||
			(game->gameSet[i][0].rank == game->gameSet[i][2].rank && game->gameSet[i][2].rank == game->gameSet[i][3].rank && game->gameSet[i][3].rank == game->gameSet[i][4].rank) ||
			(game->gameSet[i][1].rank == game->gameSet[i][2].rank && game->gameSet[i][2].rank == game->gameSet[i][3].rank && game->gameSet[i][3].rank == game->gameSet[i][4].rank)) {
			highest = FOUROFAKIND;
			continue;
		}

        // If we already have a higher rank of Full House, continue to the next combination.
		if (highest == FULLHOUSE) {
			continue;
		}

        // Check for a Full House (Three cards of one rank and two cards of another rank).
		if ((game->gameSet[i][0].rank == game->gameSet[i][1].rank && game->gameSet[i][1].rank == game->gameSet[i][2].rank && game->gameSet[i][3].rank == game->gameSet[i][4].rank) ||
			(game->gameSet[i][0].rank == game->gameSet[i][1].rank && game->gameSet[i][1].rank == game->gameSet[i][3].rank && game->gameSet[i][2].rank == game->gameSet[i][4].rank) ||
			(game->gameSet[i][0].rank == game->gameSet[i][1].rank && game->gameSet[i][1].rank == game->gameSet[i][4].rank && game->gameSet[i][2].rank == game->gameSet[i][3].rank) ||
			(game->gameSet[i][0].rank == game->gameSet[i][2].rank && game->gameSet[i][2].rank == game->gameSet[i][3].rank && game->gameSet[i][1].rank == game->gameSet[i][4].rank) ||
			(game->gameSet[i][0].rank == game->gameSet[i][2].rank && game->gameSet[i][2].rank == game->gameSet[i][4].rank && game->gameSet[i][1].rank == game->gameSet[i][3].rank) ||
			(game->gameSet[i][0].rank == game->gameSet[i][3].rank && game->gameSet[i][3].rank == game->gameSet[i][4].rank && game->gameSet[i][1].rank == game->gameSet[i][2].rank) ||
			(game->gameSet[i][1].rank == game->gameSet[i][2].rank && game->gameSet[i][2].rank == game->gameSet[i][3].rank && game->gameSet[i][0].rank == game->gameSet[i][4].rank) ||
			(game->gameSet[i][1].rank == game->gameSet[i][2].rank && game->gameSet[i][2].rank == game->gameSet[i][4].rank && game->gameSet[i][0].rank == game->gameSet[i][3].rank) ||
			(game->gameSet[i][1].rank == game->gameSet[i][3].rank && game->gameSet[i][3].rank == game->gameSet[i][4].rank && game->gameSet[i][0].rank == game->gameSet[i][2].rank) ||
			(game->gameSet[i][2].rank == game->gameSet[i][3].rank && game->gameSet[i][3].rank == game->gameSet[i][4].rank && game->gameSet[i][0].rank == game->gameSet[i][1].rank)) {
			highest = FULLHOUSE;
			continue;
		}

        // If we already have a higher rank of Flush, continue to the next combination.
		if (highest == FLUSH) {
			continue;
		}

        // Check for a Flush (All cards of the same suit).
		if (game->gameSet[i][0].suit == game->gameSet[i][1].suit && game->gameSet[i][1].suit == game->gameSet[i][2].suit && game->gameSet[i][2].suit == game->gameSet[i][3].suit && game->gameSet[i][3].suit == game->gameSet[i][4].suit) {
			highest = FLUSH;
			continue;
		}

        // If we already have a higher rank of Straight, continue to the next combination.
		if (highest == STRAIGHT) {
			continue;
		}

        // Check for a Straight (Five consecutive cards of any suit).
		if ((game->gameSet[i][0].rank == TWO && game->gameSet[i][1].rank == THREE && game->gameSet[i][2].rank == FOUR && game->gameSet[i][3].rank == FIVE && game->gameSet[i][4].rank == SIX) ||
			(game->gameSet[i][0].rank == THREE && game->gameSet[i][1].rank == FOUR && game->gameSet[i][2].rank == FIVE && game->gameSet[i][3].rank == SIX && game->gameSet[i][4].rank == SEVEN) ||
			(game->gameSet[i][0].rank == FOUR && game->gameSet[i][1].rank == FIVE && game->gameSet[i][2].rank == SIX && game->gameSet[i][3].rank == SEVEN && game->gameSet[i][4].rank == EIGHT) ||
			(game->gameSet[i][0].rank == FIVE && game->gameSet[i][1].rank == SIX && game->gameSet[i][2].rank == SEVEN && game->gameSet[i][3].rank == EIGHT && game->gameSet[i][4].rank == NINE) ||
			(game->gameSet[i][0].rank == SIX && game->gameSet[i][1].rank == SEVEN && game->gameSet[i][2].rank == EIGHT && game->gameSet[i][3].rank == NINE && game->gameSet[i][4].rank == TEN) ||
			(game->gameSet[i][0].rank == SEVEN && game->gameSet[i][1].rank == EIGHT && game->gameSet[i][2].rank == NINE && game->gameSet[i][3].rank == TEN && game->gameSet[i][4].rank == JACK) ||
			(game->gameSet[i][0].rank == EIGHT && game->gameSet[i][1].rank == NINE && game->gameSet[i][2].rank == TEN && game->gameSet[i][3].rank == JACK && game->gameSet[i][4].rank == QUEEN) ||
			(game->gameSet[i][0].rank == NINE && game->gameSet[i][1].rank == TEN && game->gameSet[i][2].rank == JACK && game->gameSet[i][3].rank == QUEEN && game->gameSet[i][4].rank == KING)) {
			highest = STRAIGHT;
			continue;
		}

        // If we already have a higher rank of Three of a Kind, continue to the next combination.
		if (highest == THREEOFAKIND) {
			continue;
		}

        // Check for Three of a Kind (Three cards with the same rank).
		if ((game->gameSet[i][0].rank == game->gameSet[i][1].rank && game->gameSet[i][1].rank == game->gameSet[i][2].rank) ||
			(game->gameSet[i][0].rank == game->gameSet[i][1].rank && game->gameSet[i][1].rank == game->gameSet[i][3].rank) ||
			(game->gameSet[i][0].rank == game->gameSet[i][1].rank && game->gameSet[i][1].rank == game->gameSet[i][4].rank) ||
			(game->gameSet[i][0].rank == game->gameSet[i][2].rank && game->gameSet[i][2].rank == game->gameSet[i][3].rank) ||
			(game->gameSet[i][0].rank == game->gameSet[i][2].rank && game->gameSet[i][2].rank == game->gameSet[i][4].rank) ||
			(game->gameSet[i][0].rank == game->gameSet[i][3].rank && game->gameSet[i][3].rank == game->gameSet[i][4].rank) ||
			(game->gameSet[i][1].rank == game->gameSet[i][2].rank && game->gameSet[i][2].rank == game->gameSet[i][3].rank) ||
			(game->gameSet[i][1].rank == game->gameSet[i][2].rank && game->gameSet[i][2].rank == game->gameSet[i][4].rank) ||
			(game->gameSet[i][1].rank == game->gameSet[i][3].rank && game->gameSet[i][3].rank == game->gameSet[i][4].rank) ||
			(game->gameSet[i][2].rank == game->gameSet[i][3].rank && game->gameSet[i][3].rank == game->gameSet[i][4].rank)) {
			highest = THREEOFAKIND;
			continue;
		}

        // If we already have a higher rank of Two Pair, continue to the next combination.
		if (highest == TWOPAIR) {
			continue;
		}

        // Check for Two Pair (Two cards of one rank and two cards of another rank).
		if ((game->gameSet[i][0].rank == game->gameSet[i][1].rank && game->gameSet[i][2].rank == game->gameSet[i][3].rank) ||
			(game->gameSet[i][0].rank == game->gameSet[i][1].rank && game->gameSet[i][2].rank == game->gameSet[i][4].rank) ||
			(game->gameSet[i][0].rank == game->gameSet[i][1].rank && game->gameSet[i][3].rank == game->gameSet[i][4].rank) ||

			(game->gameSet[i][0].rank == game->gameSet[i][2].rank && game->gameSet[i][1].rank == game->gameSet[i][3].rank) ||
			(game->gameSet[i][0].rank == game->gameSet[i][2].rank && game->gameSet[i][1].rank == game->gameSet[i][4].rank) ||
			(game->gameSet[i][0].rank == game->gameSet[i][2].rank && game->gameSet[i][3].rank == game->gameSet[i][4].rank) ||

			(game->gameSet[i][0].rank == game->gameSet[i][3].rank && game->gameSet[i][1].rank == game->gameSet[i][2].rank) ||
			(game->gameSet[i][0].rank == game->gameSet[i][3].rank && game->gameSet[i][1].rank == game->gameSet[i][4].rank) ||
			(game->gameSet[i][0].rank == game->gameSet[i][3].rank && game->gameSet[i][2].rank == game->gameSet[i][4].rank) ||

			(game->gameSet[i][0].rank == game->gameSet[i][4].rank && game->gameSet[i][1].rank == game->gameSet[i][2].rank) ||
			(game->gameSet[i][0].rank == game->gameSet[i][4].rank && game->gameSet[i][1].rank == game->gameSet[i][3].rank) ||
			(game->gameSet[i][0].rank == game->gameSet[i][4].rank && game->gameSet[i][2].rank == game->gameSet[i][3].rank) ||

			(game->gameSet[i][1].rank == game->gameSet[i][2].rank && game->gameSet[i][3].rank == game->gameSet[i][4].rank) ||
			(game->gameSet[i][1].rank == game->gameSet[i][3].rank && game->gameSet[i][2].rank == game->gameSet[i][4].rank) ||
			(game->gameSet[i][1].rank == game->gameSet[i][4].rank && game->gameSet[i][2].rank == game->gameSet[i][3].rank)) {
			highest = TWOPAIR;
			continue;
		}

        // If we already have a higher rank of One Pair, continue to the next combination.
		if (highest == ONEPAIR) {
			continue;
		}

        // Check for One Pair (Two cards with the same rank).
		if ((game->gameSet[i][0].rank == game->gameSet[i][1].rank) ||
			(game->gameSet[i][0].rank == game->gameSet[i][2].rank) ||
			(game->gameSet[i][0].rank == game->gameSet[i][3].rank) ||
			(game->gameSet[i][0].rank == game->gameSet[i][4].rank) ||
			
			(game->gameSet[i][1].rank == game->gameSet[i][2].rank) ||
			(game->gameSet[i][1].rank == game->gameSet[i][3].rank) ||
			(game->gameSet[i][1].rank == game->gameSet[i][4].rank) ||
			
			(game->gameSet[i][2].rank == game->gameSet[i][3].rank) ||
			(game->gameSet[i][2].rank == game->gameSet[i][4].rank) ||
			
			(game->gameSet[i][3].rank == game->gameSet[i][4].rank)) {
			highest = ONEPAIR;
		}
	}

	// Return the highest rank found in the combinations.
	return highest;
}

GameResult TexasHoldEm_play(TexasHoldEm* game) {
    game->hand.cards[0] = deal_card();
    game->hand.cards[1] = deal_card();
    
    game->table.cards[0] = deal_card();
    game->table.cards[1] = deal_card();
    game->table.cards[2] = deal_card();
    game->table.cards[3] = deal_card();
    game->table.cards[4] = deal_card();

    initialize_game_sets(game);

    game->gameResult.hand = game->hand;
    game->gameResult.win_condition = evaluate_hand(game);

    return game->gameResult;
}