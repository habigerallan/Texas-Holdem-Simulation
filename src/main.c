#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *workerThreadFunction(void *args) {
	workerThreadArguments *t_args = (workerThreadArguments *) args;

    GameResult *results_buffer = t_args->results_buffer;
    int worker_iterations = t_args->worker_iterations;
	pthread_mutex_t mutex = t_args->mutex;
	int offset = t_args->offset;

	TexasHoldEm game;
    for (int i = 0; i < worker_iterations; i++) {
		GameResult result = TexasHoldEm_play(&game);

		pthread_mutex_lock(&mutex);
        results_buffer[i + offset] = result;
        pthread_mutex_unlock(&mutex);
	}

	return NULL;
}

void subprocessMainFunction(GameResult *results_buffer, int iterations_to_complete, int threads_per_process, int write_end) {
	workerThreadArguments *args = malloc(sizeof(workerThreadArguments));
	args->results_buffer = results_buffer;
	args->worker_iterations = iterations_to_complete / threads_per_process;
    pthread_mutex_init(&args->mutex, NULL);

	int total_offset = 0;
	pthread_t threads[threads_per_process];
    for (int i = 0; i < threads_per_process; i++) {
		if (i+1 == threads_per_process) {
			args->worker_iterations = iterations_to_complete % threads_per_process;
		}

		args->offset = total_offset;
		total_offset += args->worker_iterations;

        if (pthread_create(&threads[i], NULL, workerThreadFunction, (void *) args) != 0) {
            perror("Failed to create worker thread");
			free(args);
            exit(EXIT_FAILURE);
        }
    }
	
    // Wait for worker threads to finish
    for (int i = 0; i < threads_per_process - 1; i++) {
        pthread_join(threads[i], NULL);
    }
    
	// Now write into the pipe
    for (int i = 0; i < iterations_to_complete; i++) {
        if (write(write_end, &results_buffer[i], sizeof(GameResult)) == -1) {
            perror("Error writing to pipe");
			free(args);
            exit(EXIT_FAILURE);
        }
    }

	free(args);
	return;
}

void *mainThreadFunction(void *args) {
    ThreadArguments *t_args = (ThreadArguments *) args;

    HandMap hand_map = t_args->hand_map;
    int iterations = t_args->iterations;
    int processes_count = t_args->processes_count;
    int threads_per_process = t_args->threads_per_process;

    pid_t pid;
    int max_fd = 0;
    fd_set readfds;
    FD_ZERO(&readfds);

    int pipefds[processes_count][2];  // Array to store pipes for each subprocess

    for (int i = 0; i < processes_count; i++) {
        if (pipe(pipefds[i]) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }

        pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) {  // Child process
            close(pipefds[i][0]);  // Close the read end of the pipe

			GameResult *results_buffer = malloc(iterations * sizeof(GameResult));
			if (!results_buffer) {
				perror("Failed to allocate memory for results");
				exit(1);
			}

			subprocessMainFunction(results_buffer, iterations, threads_per_process, pipefds[i][1]);

            close(pipefds[i][1]);  // Close the write end of the pipe
			free(results_buffer);
            exit(EXIT_SUCCESS);
        } else {  // Parent process
            close(pipefds[i][1]);  // Close the write end of the pipe

            if (pipefds[i][0] > max_fd) {
                max_fd = pipefds[i][0];
            }

            FD_SET(pipefds[i][0], &readfds);  // Add the read end of the pipe to the readfds set
        }
    }

    int open_fd_count = processes_count;

	while (open_fd_count > 0) {
		fd_set tempfds = readfds;
		int activity = select(max_fd + 1, &tempfds, NULL, NULL, NULL);

		if (activity < 0) {
			perror("select");
			exit(EXIT_FAILURE);
		}

		for (int i = 0; i < processes_count; i++) {
			if (FD_ISSET(pipefds[i][0], &tempfds)) {
				GameResult result;
				ssize_t readBytes = read(pipefds[i][0], &result, sizeof(GameResult));
				if (readBytes > 0) {
					add_game_result(hand_map, result);
				} else if (readBytes == 0) {
					close(pipefds[i][0]);  // Close the pipe if no more data
					FD_CLR(pipefds[i][0], &readfds);  // Remove from set
					open_fd_count--;  // Decrement the open file descriptor count
				}
			}
		}
	}

    // Wait for all child processes to finish
    while (wait(NULL) > 0) {
    // Loop will continue until there are no more child processes
	}
	
	return NULL;
}

int main(int argc, char *argv[]) {
	if (argc != 4) {
        fprintf(stderr, "Usage: %s <total_iterations> <total_processes> <threads_per_process>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int total_iterations = atoi(argv[1]);
    int total_processes = atoi(argv[2]);
    int threads_per_process = atoi(argv[3]);

    if (total_iterations <= 0 || total_processes <= 0 || threads_per_process <= 0) {
        fprintf(stderr, "Arguments must be positive integers.\n");
        return EXIT_FAILURE;
    }

	ThreadArguments args;
	args.hand_map = init_card_data();
	args.iterations = total_iterations / total_processes;
	args.processes_count = total_processes / threads_per_process;
	args.threads_per_process = threads_per_process;

	pthread_t threads[threads_per_process];
    for (int i = 0; i < threads_per_process; i++) {
		if (i+1 == threads_per_process) {
			args.iterations += total_iterations % total_processes;
			args.processes_count += total_processes % threads_per_process;
		}

		if (pthread_create(&threads[i], NULL, mainThreadFunction, (void *) &args) != 0) {
			perror("Failed to create thread");
			exit(1);
		}
    }

	for (int i = 0; i < threads_per_process; i++) {
        pthread_join(threads[i], NULL);
    }

	// read handmap into some sort of file (maybe json **RESEARCH** )

	printf("Complete!");

	free_card_data(args.hand_map);

	return 0;
}
