#include "card.h"

void initialize_random() {
    srand((unsigned int)time(NULL));
}

Card deal_card() {
    Card card;
    card.suit = rand() % 4 + 1;
    card.rank = rand() % 13 + 2;
    return card;
}
