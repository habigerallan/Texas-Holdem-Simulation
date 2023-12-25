#include "TexasHoldEm.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdatomic.h>
#include <sys/time.h>

// Define the hash map structure and related functions
typedef struct {
    int win_conditions[10];
	int hand_count;
    pthread_mutex_t mutex;
} ResultNode;

// 1378 is total possible unique poker hands
ResultNode *cardData;

atomic_int currentIterations = 0;
int totalIterations;
int numThreads;

const char* suit_to_string(int suit) {
    switch (suit) {
        case HEARTS: return "Hearts";
        case DIAMONDS: return "Diamonds";
        case CLUBS: return "Clubs";
        case SPADES: return "Spades";
        default: return "Unknown Suit";
    }
}


const char* rank_to_string(int rank) {
    switch (rank) {
        case TWO: return "Two";
        case THREE: return "Three";
        case FOUR: return "Four";
        case FIVE: return "Five";
        case SIX: return "Six";
        case SEVEN: return "Seven";
        case EIGHT: return "Eight";
        case NINE: return "Nine";
        case TEN: return "Ten";
        case JACK: return "Jack";
        case QUEEN: return "Queen";
        case KING: return "King";
        case ACE: return "Ace";
        default: return "Unknown Rank";
    }
}

Card index_to_card(int index) {
    Card card;
    card.suit = index / 13 + 1;
    card.rank = index % 13 + 2;
    return card;
}

void get_cards_from_index(int uniqueIndex, Card *card1, Card *card2) {
    int index1 = 0;
    while (index1 * (52 - index1) / 2 <= uniqueIndex) {
        index1++;
    }
    index1--;

    int index2 = uniqueIndex - index1 * (52 - index1) / 2 + index1;

    *card1 = index_to_card(index1);
    *card2 = index_to_card(index2);
}

int card_to_index(Card card) {
    return (card.suit - 1) * 13 + (card.rank - 2);
}

// Function to map two cards to a unique hand index
int get_hand_index(Card card1, Card card2) {
    int index1 = card_to_index(card1);
    int index2 = card_to_index(card2);

    // Ensure index1 is always the smaller index
    if (index1 > index2) {
        int temp = index1;
        index1 = index2;
        index2 = temp;
    }

    // Map the pair of indices to a unique index
    return index1 * (52 - index1) / 2 + index2 - index1;
}

void init_cardData() {
    int size = 1326 + 52;  // 1326 for unique pairs, 52 for identical pairs
    cardData = malloc(size * sizeof(ResultNode));

    // Initialize each ResultNode in the array
    for (int i = 0; i < size; i++) {
        cardData[i].hand_count = 0;
        pthread_mutex_init(&(cardData[i].mutex), NULL);
		cardData[i].win_conditions[0] = 0;
		cardData[i].win_conditions[1] = 0;
		cardData[i].win_conditions[2] = 0;
		cardData[i].win_conditions[3] = 0;
		cardData[i].win_conditions[4] = 0;
		cardData[i].win_conditions[5] = 0;
		cardData[i].win_conditions[6] = 0;
		cardData[i].win_conditions[7] = 0;
		cardData[i].win_conditions[8] = 0;
        cardData[i].win_conditions[9] = 0;
    }
}

void free_cardData() {
    int size = 1326 + 52;  // 1326 for unique pairs, 52 for identical pairs
	for (int i = 0; i < size; i++) {
        pthread_mutex_destroy(&(cardData[i].mutex));
    }

    free(cardData);
}

void ProcessResults(GameResult result) {
    int index = get_hand_index(result.hand.cards[0], result.hand.cards[1]);

    pthread_mutex_lock(&cardData[index].mutex);
    cardData[index].win_conditions[result.win_condition]++;
    cardData[index].hand_count++;
    pthread_mutex_unlock(&cardData[index].mutex);
}

void *progressThread() {
	int barWidth = 50;
    while (currentIterations < totalIterations) {
        double progress = (double)currentIterations / totalIterations;
        int progressBar = (int)(progress * barWidth);

        printf("Progress: [");
        for (int j = 0; j < progressBar; ++j) {
            printf("=");
        }
        for (int j = progressBar; j < barWidth; ++j) {
            printf(" ");
        }
        printf("] %.1f%%\r", progress * 100.0);
        fflush(stdout); // Flush the output buffer

        usleep(100000); // Sleep for 100 milliseconds
    }

	double progress = (double)currentIterations / totalIterations;
    int progressBar = (int)(progress * barWidth);

    printf("Progress: [");
    for (int j = 0; j < progressBar; ++j) {
        printf("=");
    }
    for (int j = progressBar; j < barWidth; ++j) {
        printf(" ");
    }
    printf("] %.1f%%\n", progress * 100.0);

    usleep(100000); // Sleep for 100 milliseconds
    return NULL;
}

void *thread_function() {
    TexasHoldEm game;
    while (atomic_load(&currentIterations) < totalIterations) {
        GameResult result = TexasHoldEm_play(&game);
        ProcessResults(result);
        atomic_fetch_add(&currentIterations, 1); // Atomic increment
    }
    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <totalIterations> <numThreads>\n", argv[0]);
        return 1;
    }

    totalIterations = atoi(argv[1]);
    numThreads = atoi(argv[2]);

    if (totalIterations <= 0 || numThreads <= 0) {
        fprintf(stderr, "Error: Invalid arguments. Both arguments must be positive integers.\n");
        return 1;
    }

    initialize_random();
	init_cardData();
    
    struct timeval start_t, end_t;
    double total_t;
    pthread_t *threads = malloc(numThreads * sizeof(pthread_t));
    pthread_t progressBarThread;

    // Start the progress bar thread
    pthread_create(&progressBarThread, NULL, progressThread, NULL);

    // Get the start time
    gettimeofday(&start_t, NULL);

    // Start worker threads
    for (int i = 0; i < numThreads; ++i) {
        pthread_create(&threads[i], NULL, thread_function, NULL);
    }

    // Wait for all threads to finish
    for (int i = 0; i < numThreads; ++i) {
        pthread_join(threads[i], NULL);
    }

    // Get the end time
    gettimeofday(&end_t, NULL);
    
    // Wait for the progress bar thread to finish
    pthread_join(progressBarThread, NULL);

    // Calculate and print the execution time
    total_t = (end_t.tv_sec - start_t.tv_sec) + (end_t.tv_usec - start_t.tv_usec) / 1000000.0;
	printf("Execution Time: %.2f seconds\n", total_t);
    printf("Iteratons Per Second: %.2f\n", totalIterations / total_t);

    // Output the results to a file
    FILE *outputFile = fopen("./output/hand_data.txt", "w");
    if (!outputFile) {
        fprintf(stderr, "Failed to open the output file.\n");
        return 1;
    }

    for (int i = 0; i < 1378; ++i) {
		Card card1, card2;
		get_cards_from_index(i, &card1, &card2);

		fprintf(outputFile, "Hand: %d of %d, %s of %s\n", card1.rank, card1.suit, rank_to_string(card2.rank), suit_to_string(card2.suit));
		for (int j = 0; j < 10; ++j) {
			double avg_win_rate = 0.0;
			if (cardData[i].hand_count > 0) {
				avg_win_rate = ((double)cardData[i].win_conditions[j]) / cardData[i].hand_count * 100.0;
			}

			fprintf(outputFile, "Win Condition %d: %d times. Win rate: %0.2f%%\n", j, cardData[i].win_conditions[j], avg_win_rate);
		}

		fprintf(outputFile, "Total times hand seen: %d times\n", cardData[i].hand_count);
	}

    fclose(outputFile);
    free(threads);
	free_cardData();

    return 0;
}
