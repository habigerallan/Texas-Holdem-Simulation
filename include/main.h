#ifndef MAIN_H
#define MAIN_H

#include <stdatomic.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include "texas_hold_em.h"
#include "map.h"

typedef struct {
	HandMap hand_map;
	atomic_int total_iterations;
	atomic_int current_iterations;
} SharedData;

typedef struct {
	int processes_count;
	int threads_per_process;
} ThreadArgs;

typedef struct {
	GameResult *results_buffer;
	int worker_iterations;
	int offset;
	pthread_mutex_t mutex;
} workerThreadArguments;

void *ThreadFunction(void *args);


// typedef struct {
// 	HandMap hand_map;
// 	int iterations;
// 	int processes_count;
// 	int threads_per_process;
// } ThreadArguments;

// typedef struct {
// 	GameResult *results_buffer;
// 	int worker_iterations;
// 	int offset;
// 	pthread_mutex_t mutex;
// } workerThreadArguments;

// void *workerThreadFunction(void *args);
// void subprocessMainFunction(GameResult *results_buffer, int iterations_to_complete, int threads_per_process, int write_end);
// void *mainThreadFunction(void *args);

#endif