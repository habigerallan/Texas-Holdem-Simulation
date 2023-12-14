#include "TexasHoldEm.hpp"

TexasHoldEm::TexasHoldEm() : rng(std::random_device()()) {}

const Hand& TexasHoldEm::get_hand() const {
    return hand;
}

const Table& TexasHoldEm::get_table() const {
    return table;
}

bool TexasHoldEm::play() {
	table.cards[0] = deal_card();
	table.cards[1] = deal_card();
	table.cards[2] = deal_card();
	table.cards[3] = deal_card();
	table.cards[4] = deal_card();
	hand.cards[0] = deal_card();
	hand.cards[1] = deal_card();
	return evaluate_hand();
}
