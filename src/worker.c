#include "worker.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>           // For O_* constants
#include <sys/mman.h>        // For shared memory
#include <unistd.h>          // For ftruncate

void* ThreadFunction(void *args) {
    // Unpack the thread argument which is the shared memory
    SharedData *shared_data = (SharedData *)args;
	TexasHoldEm game;

	int curr = atomic_load(&(shared_data->current_iterations));
	int total = atomic_load(&(shared_data->total_iterations));

	while (curr < total) {
        GameResult game_result = TexasHoldEm_play(&game);
        add_game_result(shared_data->hand_map, game_result);
		printf("success: %d\n", curr);
		atomic_fetch_add(&(shared_data->current_iterations), 1);
		curr = atomic_load(&(shared_data->current_iterations));
    }

    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <threads_per_process>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int threads_per_process = atoi(argv[1]);
    if (threads_per_process <= 0) {
        fprintf(stderr, "Invalid number of threads per process\n");
        return EXIT_FAILURE;
    }

	key_t key = ftok("somefile", 65); // Use the same key as in Program 1
    int shm_id = shmget(key, sizeof(SharedData), 0666); // Connect to the shared memory segment
	if (shm_id < 0) {
        perror("shmget");
        exit(1);
    }

    SharedData *shared_data = (SharedData *) shmat(shm_id, (void*)0, 0);

    pthread_t threads[threads_per_process];

    for (int i = 0; i < threads_per_process; ++i) {
        if (pthread_create(&threads[i], NULL, ThreadFunction, shared_data) != 0) {
            perror("Failed to create thread");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < threads_per_process; ++i) {
        pthread_join(threads[i], NULL);
    }

    // Clean up
    shmdt(shared_data);

    return EXIT_SUCCESS;
}


// void subprocessMainFunction(GameResult *results_buffer, int iterations_to_complete, int threads_per_process, int write_end) {
// 	workerThreadArguments *args = malloc(sizeof(workerThreadArguments));
// 	args->results_buffer = results_buffer;
// 	args->worker_iterations = iterations_to_complete / threads_per_process;
//     pthread_mutex_init(&args->mutex, NULL);

// 	int total_offset = 0;
// 	pthread_t threads[threads_per_process];
//     for (int i = 0; i < threads_per_process; i++) {
// 		if (i+1 == threads_per_process) {
// 			args->worker_iterations += iterations_to_complete % threads_per_process;
// 		}

// 		args->offset = total_offset;
// 		total_offset += args->worker_iterations;
// 		printf("complete, %d\n", args->worker_iterations);

//         if (pthread_create(&threads[i], NULL, workerThreadFunction, (void *) args) != 0) {
//             perror("Failed to create worker thread");
// 			free(args);
//             exit(EXIT_FAILURE);
//         }
//     }
	
//     // Wait for worker threads to finish
//     for (int i = 0; i < threads_per_process - 1; i++) {
//         pthread_join(threads[i], NULL);
//     }
    
// 	// Now write into the pipe
//     for (int i = 0; i < iterations_to_complete; i++) {
//         if (write(write_end, &results_buffer[i], sizeof(GameResult)) == -1) {
//             perror("Error writing to pipe");
// 			free(args);
//             exit(EXIT_FAILURE);
//         }
//     }

// 	free(args);
// 	return;
// }