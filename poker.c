#include "hash.h"
#include "poker.h"
#include <unistd.h>
#include <pthread.h>
#include <sys/wait.h>

int player_count = 4
int processes = 24;

int process_threads = 8;
int main_threads = 4;

int total_iterations = 100000;

int thread_iterations = (total_iterations / processes) / process_threads;
int thread_processes = processes / main_threads;

HashTable *card_win_rates;

void *child_thread_function(void *arg) {
    int pipefd = *(int *)arg;
    int result = play_chunk(thread_iterations); 

    write(pipefd, &result, sizeof(result));

    return NULL;
}

void *thread_function() {
	int pipefd[2];

    for (int i = 0; i < thread_processes; i++) {

        if (pipe(pipefd) == -1) {
			perror("pipe");
			exit(EXIT_FAILURE);
		}

		pid_t pid = fork();
		if (pid == -1) {
			perror("fork");
			exit(EXIT_FAILURE);
		} 
		
		if (pid == 0) {
			close(pipefd[0]);

			pthread_t threads[process_threads];
			for (int i = 0; i < process_threads; i++) {
				if (pthread_create(&threads[i], NULL, child_thread_function, &pipefd[1]) != 0) {
					perror("Failed to create thread");
					exit(EXIT_FAILURE);
				}
			}

			for (int i = 0; i < process_threads; i++) {
				pthread_join(threads[i], NULL);
			}

			close(pipefd[1]);
			exit(0);
		} else {
			close(pipefd[1]);

			int result;
			while (read(pipefd[0], &result, sizeof(result)) > 0) {
				// insert result which is the winning players 2 cards into the hashtable
			}

			close(pipefd[0]);
			wait(NULL);
		}
	}

    return NULL;
}

int main() {
	card_win_rates = createHashTable(2652);

	pthread_t threads[main_threads];
    for (int i = 0; i < main_threads; i++) {
        if (pthread_create(&threads[i], NULL, thread_functon) != 0) {
            perror("Failed to create thread");
            return 1;
        }
    }

    for (int i = 0; i < main_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}

Card deal_card() {
    Card card;
    card.suit = rand() % 4 + 1;
    card.rank = rand() % 13 + 2;
    return card;
}

void *play_chunk(void *arg) {
    int iterations = *(int *)arg;
	Player players[player_count];
    Game game;

    for (int i = 0; i < iterations; i++) {
		for (int j = 0; j < players_count; j++) {
            players[j].hand.cards[0] = deal_card();
            players[j].hand.cards[1] = deal_card();
        }

		for (int j = 0; j < 3; j++) {
            game.flop[j] = deal_card();
        }
        game.turn = deal_card();
        game.river = deal_card();

		Player *winning_player = evaluate_hand(players, game);
        
        Hand *winning_hand = malloc(sizeof(Hand));
        *winning_hand = winning_player->hand;
        return winning_hand;
    }

    return NULL;
}

int evaluate_hand(Player *players, Game *game) {
    return 0;
}
