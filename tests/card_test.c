#include "card.h"
#include <assert.h>
#include <stdio.h>

#define CARD_DRAWS 100

void test_deal_card() {
    for (int i = 0; i < CARD_DRAWS; i++) {
        Card c = deal_card();
        assert(c.suit >= HEARTS && c.suit <= SPADES);
        assert(c.rank >= TWO && c.rank <= ACE);
    }
}

void get_card_list(Card *card_list) {
    for (int i = 0; i < CARD_DRAWS; i++) {
        Card c = deal_card();
        card_list[i] = c;
    }
}

int compare_card_lists(Card *list1, Card *list2) {
    for (int i = 0; i < CARD_DRAWS; i++) {
        if (list1[i].suit != list2[i].suit || list1[i].rank != list2[i].rank) {
            return 0; 
        }
    }
    return 1; 
}

int main() {
    printf("Running tests...\n");

    initialize_random();
    test_deal_card();

    Card card_list1[CARD_DRAWS];
    initialize_random(); 
    get_card_list(card_list1)

    Card card_list2[CARD_DRAWS];
    initialize_random(); 
    get_card_list(card_list2)

    assert(!compare_card_lists(card_list1, card_list2));

    printf("All tests passed successfully.\n");
    return 0;
}
