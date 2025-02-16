#include "processes.h"

// global variables
pid_t background_processes[BACKGROUND_PROCESS_LIMIT]; // array holding PIDs of still incomplete background processes
int num_background_processes = 0;

/**
 * Adds a process's PID to the tracking array
 * @param pid_t the process's PID
*/
void add_process(pid_t pid) {
	if (num_background_processes < BACKGROUND_PROCESS_LIMIT) {
		background_processes[num_background_processes] = pid;
		num_background_processes++;
	} else {
		printf("ERROR: Background process limit reached.");
		fflush(stdout);
	}
}

/**
 * Terminates all background processes
*/
void terminate_processes() {
	for (int i = 0; i < num_background_processes; i++) {
		kill(background_processes[i], SIGTERM);
	}
}
