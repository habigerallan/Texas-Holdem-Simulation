#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void workerThreadFunction(GameResult *results_buffer, int worker_iterations) {
}

void pipeThreadFunction(GameResult *results_buffer, int write_end) {
}

void subprocessMainFunction(GameResult *results_buffer, int iterations_to_complete, int threads_per_process, int write_end) {
    pthread_t threads[threads_per_process - 1];

    // Create worker threads
    void *args[] = {results_buffer, &iterations_to_complete};
    for (int i = 0; i < threads_per_process - 1; i++) {
        if (pthread_create(&threads[i], NULL, workerThreadFunction, args) != 0) {
            perror("Failed to create worker thread");
            exit(EXIT_FAILURE);
        }
    }

    // Create pipe thread
    pthread_t pipe_thread;
    void *pipe_args[] = {results_buffer, &write_end};
    if (pthread_create(&pipe_thread, NULL, pipeThreadFunction, pipe_args) != 0) {
        perror("Failed to create pipe thread");
        exit(EXIT_FAILURE);
    }

    // Wait for worker threads to finish
    for (int i = 0; i < threads_per_process - 1; i++) {
        pthread_join(threads[i], NULL);
    }

    // Wait for pipe thread to finish
    pthread_join(pipe_thread, NULL);
}

void mainThreadFunction(void *args) {
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

    // Select loop to handle reading from multiple pipes
    while (1) {
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
                }
            }
        }

        // Check if all pipes are closed
        if (FD_ISZERO(&readfds)) {
            break;
        }
    }

    // Wait for all child processes to finish
    while (wait(NULL) > 0);
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

	printf("Complete!");

	free_card_data(args.hand_map);
}
