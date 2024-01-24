#include "main.h"

void* ThreadFunction(void *args) {
    ThreadArgs *t_args = (ThreadArgs *) args;
    int processes_count = t_args->processes_count;
    int threads_per_process = t_args->threads_per_process; // May not be needed
    int i;

    for (i = 0; i < processes_count; ++i) {
        pid_t pid = fork();

        // Child process
        if (pid == 0) {
            char threads_str[16];
            snprintf(threads_str, sizeof(threads_str), "%d", threads_per_process);
            execl("./build/worker", "worker", threads_str, (char *) NULL);
            perror("execl");
            exit(EXIT_FAILURE);
        } else if (pid < 0) {
            // Handle error in fork
            perror("fork");
            exit(EXIT_FAILURE);
        }
    }

    // Parent process waits for all child processes to finish
    for (i = 0; i < processes_count; ++i) {
        wait(NULL);
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

	ThreadArgs args;

	key_t key = ftok("somefile", 65); // Create a unique key. 'somefile' should be an existing file.
    int shm_id = shmget(key, sizeof(SharedData), 0666|IPC_CREAT); // Create the shared memory segment

    // Error checking
    if (shm_id < 0) {
        perror("shmget");
        exit(1);
    }

	SharedData *shared_data = (SharedData *) shmat(shm_id, (void*)0, 0);

	shared_data->hand_map = init_card_data();
    shared_data->total_iterations = total_iterations;
    shared_data->current_iterations = 0;

	args.processes_count = total_processes / threads_per_process;
	args.threads_per_process = threads_per_process;

	pthread_t threads[threads_per_process];
    for (int i = 0; i < threads_per_process; i++) {
		if (i+1 == threads_per_process) {
			args.processes_count += total_processes % threads_per_process;
		}

		if (pthread_create(&threads[i], NULL, ThreadFunction, (void *) &args) != 0) {
			perror("Failed to create thread");
			exit(1);
		}
    }

	for (int i = 0; i < threads_per_process; i++) {
        pthread_join(threads[i], NULL);
    }

	// TODO //
	// print shared memory current iterations
	int curr = shared_data->current_iterations;
	int total = shared_data->total_iterations;
	while (curr < total) {
		printf("%d\n", curr);
		sleep(1);
		curr = shared_data->current_iterations;
	}

	if (shmdt(shared_data) == -1) {
        perror("shmdt");
        exit(1);
    }

    // Mark the segment to be destroyed
    if (shmctl(shm_id, IPC_RMID, NULL) == -1) {
        perror("shmctl");
        exit(1);
    }

    munmap(shared_data, sizeof(SharedData));

	return 0;
}
