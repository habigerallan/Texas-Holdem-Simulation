#include "main.h"
#define PROGRESS_BAR_WIDTH 50 // Width of the progress bar in characters

void* progressbar(void *args) {
    SharedData *shared_data = (SharedData *)args;

    // Initial print out of the progress bar
    printf("[");
    for (int i = 0; i < PROGRESS_BAR_WIDTH; i++) {
        printf(" ");
    }
    printf("]\r["); // Carriage return to print over the same line

    while (atomic_load(&shared_data->current_iterations) < atomic_load(&shared_data->total_iterations)) {
        int current_progress = atomic_load(&shared_data->current_iterations);
        int total = atomic_load(&shared_data->total_iterations);
        int progress_percentage = (current_progress * 100) / total;
        int pos = (progress_percentage * PROGRESS_BAR_WIDTH) / 100;

        // Update the progress bar
        for (int i = 0; i < pos; i++) {
            printf("=");
        }
        for (int i = pos; i < PROGRESS_BAR_WIDTH; i++) {
            printf(" ");
        }
        printf("] %d%%\r", progress_percentage); // Carriage return to print over the same line
        fflush(stdout); // Flush the output buffer to ensure it prints immediately

        // Sleep for a short interval so you don't update too frequently
        usleep(100000); // Sleep for 100 milliseconds
    }

    // Final print out to ensure the progress bar shows as complete
    printf("[");
    for (int i = 0; i < PROGRESS_BAR_WIDTH; i++) {
        printf("=");
    }
    printf("] 100%%\n"); // Newline at the end

    return NULL;
}

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
	SharedData *shared_data;
	
	int shm_fd = shm_open("/mySharedMemory", O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, sizeof(SharedData));
    shared_data = mmap(0, sizeof(SharedData), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
	
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

	pthread_t threadsasd;
	if (pthread_create(&threadsasd, NULL, progressbar, (void *) &shared_data) != 0) {
		perror("Failed to create thread");
		exit(1);
	}

	for (int i = 0; i < threads_per_process; i++) {
        pthread_join(threads[i], NULL);
    }

	pthread_join(threadsasd, NULL);

	// read handmap into some sort of file (maybe json **RESEARCH** )

	printf("Complete!");
    munmap(shared_data, sizeof(SharedData));
    close(shm_fd);
    shm_unlink("/mySharedMemory");

	return 0;
}
