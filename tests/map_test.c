#include "map.h"
#include <assert.h>
#include <stdio.h>

HandMap test_init_card_data() {
    HandMap handMap = init_card_data();

    // Test if card_data is not NULL
    assert(handMap.card_data != NULL);

    for (int i = 0; i < 52; i++) {
        // Test if each row in card_data is not NULL
        assert(handMap.card_data[i] != NULL);

        for (int j = 0; j < (52 - i); j++) {
            Node node = handMap.card_data[i][j];

            // Test if win_conditions are all initialized to 0
            for (int k = 0; k < 10; k++) {
                assert(node.win_conditions[k] == 0);
            }

            // Test if hand_count is initialized to 0
            assert(node.hand_count == 0);

            // Test if the mutex is properly initialized
            assert(pthread_mutex_trylock(&node.map_mutex) == 0);
            pthread_mutex_unlock(&node.map_mutex);
        }
    }

    return handMap;
}

void test_free_card_data(HandMap handMap) {
    free_card_data(handMap);
    assert(handMap.card_data == NULL);
}

void test_indexing(HandMap handMap) {
    int correct_index1 = 0;
    int correct_index2 = 0;
    for (int i1 = 1; i1 < 5; i1++) {
        for (int j1 = 2; j1 < 15; j1++) {
            Card card1;
            card1.suit = i1;
            card1.rank = j1;
            for (int i2 = 1; i2 < 5; i2++) {
                for (int j2 = 2; j2 < 15; j2++) {
                    Card card2;
                    card2.suit = i2;
                    card2.rank = j2;

                    Hand hand = {card1, card2};
                    Hand alt_hand = {card2, card1};
                    NodeIndex node_index = hand_to_index(hand);

                    assert(correct_index1 == node_index.card1_index);
                    assert(correct_index2 == node_index.card2_index);

                    Hand indexed_hand = index_to_hand(node_index);
                    assert(indexed_hand == hand || indexed_hand == alt_hand);

                    correct_index2 += 1;
                }
            }

            correct_index1 += 1
            correct_index2 = 0;
        }
    }
}

// void test_add_game_results() {

// }

// void test_get_game_results() {

// }

int main() {
    printf("Running tests...\n");

    HandMap hand_map = test_init_card_data();

    test_indexing(hand_map);

    //test_add_game_results();
    //test_get_game_results();

    test_free_card_data(hand_map);

    printf("All tests passed successfully.\n");
    return 0;
}
