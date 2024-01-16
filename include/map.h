#ifndef MAP_H
#define MAP_H

#include "texas_hold_em.h"
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/select.h>
#include <pthread.h>
#include "card.h"

typedef struct {
    int win_conditions[10];
	int hand_count;
	pthread_mutex_t map_mutex;
} Node;

typedef struct {
    int card1_index;
	int card2_index;
} NodeIndex;

typedef struct {
	Node **card_data;
} HandMap;

HandMap init_card_data();
void free_card_data(HandMap hand_map);

NodeIndex hand_to_index(Hand hand);
Hand index_to_hand(NodeIndex index);

void add_game_result(HandMap hand_map, GameResult game_result);
Node get_game_result(HandMap hand_map, NodeIndex index);

#endif