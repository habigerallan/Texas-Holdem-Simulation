#include "map.h"

HandMap init_card_data() {
	HandMap hand_map;
    Node **card_data;

    card_data = malloc(52 * sizeof(Node *));
    if (!card_data) {
        perror("Failed to allocate memory for card_data");
        exit(EXIT_FAILURE);
    }
	
    for (int i = 0; i < 52; i++) {
		card_data[i] = malloc((52 - i) * sizeof(Node));
        if (!card_data[i]) {
            perror("Failed to allocate memory for card_data row");
            exit(EXIT_FAILURE);
        }

		for (int j = 0; j < (52 - i); j++) {
            for (int k = 0; k < 10; k++) {
                card_data[i][j].win_conditions[k] = 0;
            }

            card_data[i][j].hand_count = 0;
			
            pthread_mutex_init(&card_data[i][j].map_mutex, NULL);
        }
    }

	hand_map.card_data = card_data;

	return hand_map;
}

void free_card_data(HandMap hand_map) {
    for (int i = 0; i < 52; i++) {
		for (int j = 0; j < (52 - i); j++) {
			pthread_mutex_destroy(&(hand_map.card_data[i][j].map_mutex));
		}

        free(hand_map.card_data[i]);
    }

    free(hand_map.card_data);
}

NodeIndex hand_to_index(Hand hand) {
	Card card1 = hand.cards[0];
    Card card2 = hand.cards[1];

	if (card1.suit > card2.suit || (card1.rank == card2.rank && card1.suit > card2.suit)) {
        Card temp = card1;
        card1 = card2;
        card2 = temp;
    }

	int rank1 = card1.rank - 2;
	int suit1 = card1.suit - 1;
	
	int rank2 = card2.rank - 2;
	int suit2 = card2.suit - 1;

	int card1_index = suit1 * 13 + rank1;
	int card2_index = (suit2 * 13 + rank2) - card1_index;

	NodeIndex index;
	index.card1_index = card1_index;
	index.card2_index = card2_index;
	
	return index;
}

Hand index_to_hand(NodeIndex index) {
    Hand hand;
    Card card1, card2;

    int suit1 = index.card1_index / 13;
    int rank1 = index.card1_index % 13;

    card1.suit = suit1 + 1;
    card1.rank = rank1 + 2;

    int suit2 = index.card2_index / 13;
    int rank2 = index.card2_index % 13;

    card2.suit = suit2 + 1;
    card2.rank = rank2 + 2;

    hand.cards[0] = card1;
    hand.cards[1] = card2;

    return hand;
}

void add_game_result(HandMap hand_map, GameResult game_result) {
	Node **card_data = hand_map.card_data;
	Hand hand = game_result.hand;
	int win_condition = game_result.win_condition;

	NodeIndex index = hand_to_index(hand);
    int index1 = index.card1_index;
    int index2 = index.card2_index;
	if (index1 < 0 || index1 >= 52) {
        fprintf(stderr, "Index1 out of bounds: %d\n", index1);
        return;
    }
	printf("1/3\n");

    if (!card_data[index1]) {
        fprintf(stderr, "card_data[%d] is NULL\n", index1);
        return;
    }
	printf("2/3\n");
    Node *current_node_ptr = &card_data[index1][index2];
	printf("3/3\n");

	pthread_mutex_lock(&current_node_ptr->map_mutex);
    current_node_ptr->win_conditions[win_condition]++;
    current_node_ptr->hand_count++;
    pthread_mutex_unlock(&current_node_ptr->map_mutex);
}

Node get_game_result(HandMap hand_map, NodeIndex index) {
    Node **card_data = hand_map.card_data;
    int index1 = index.card1_index;
    int index2 = index.card2_index;

    pthread_mutex_lock(&card_data[index1][index2].map_mutex);
    Node result = card_data[index1][index2];
    pthread_mutex_unlock(&card_data[index1][index2].map_mutex);

    return result;
}