#include "TexasHoldEm.hpp"
#include <iostream>

void print_card(const Card& card) {
    const char* suits[] = {"Hearts", "Diamonds", "Clubs", "Spades"};
    const char* ranks[] = {"", "", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten", "Jack", "Queen", "King", "Ace"};

    std::cout << ranks[card.rank] << " of " << suits[card.suit - 1] << std::endl;
}

int main() {
    TexasHoldEm game;
    game.play();

    // Get the hand and table from the game
    const Hand& hand = game.get_hand();
    const Table& table = game.get_table();

    // Print the hand
    std::cout << "Hand:" << std::endl;
    print_card(hand.cards[0]);
    print_card(hand.cards[1]);

    // Print the table cards
    std::cout << "Table:" << std::endl;
    for (const Card& card : table.cards) {
        print_card(card);
    }

    return 0;
}
