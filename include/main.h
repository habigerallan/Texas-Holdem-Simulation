#ifndef MAIN_H
#define MAIN_H

#include "texas_hold_em.h"
#include <unistd.h>
#include "map.h"

typedef struct {
	HandMap hand_map;
	int iterations;
	int processes_count;
	int threads_per_process;
} ThreadArguments;

void workerThreadFunction(GameResult *results_buffer, int iterations_to_complete);
void pipeThreadFunction(GameResult *results_buffer, int write_end);
void subprocessMainFunction(GameResult *results_buffer, int iterations_to_complete, int threads_per_process, int write_end);
void mainThreadFunction(void *args);

#endif