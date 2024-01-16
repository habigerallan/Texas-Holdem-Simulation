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
	while (atomic_load(&shared_data->current_iterations) < atomic_load(&shared_data->total_iterations)) {
        GameResult game_result = TexasHoldEm_play(&game);
        add_game_result(shared_data->hand_map, game_result);
        atomic_fetch_add(&shared_data->current_iterations, 1);
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

    // Open shared memory in read/write mode
    int shm_fd = shm_open("/mySharedMemory", O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    // Map shared memory
    SharedData *shared_data = mmap(NULL, sizeof(SharedData), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shared_data == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

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
    munmap(shared_data, sizeof(SharedData));
    close(shm_fd);

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