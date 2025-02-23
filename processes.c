#include "processes.h"

// define global variables
pid_t background_processes[BACKGROUND_PROCESS_LIMIT]; // array holding PIDs of still incomplete background processes
int num_background_processes = 0;

/**
 * Adds a process's PID to the tracking array.
 * @param pid: pid_t, the process's PID
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
 * Terminates all background processes.
 */
void terminate_processes() {
	for (int i = 0; i < num_background_processes; i++) {
		kill(background_processes[i], SIGTERM);
	}
	num_background_processes = 0; // reset tracker
}

/**
 * Checks whether any background processes have completed.
 */
void check_background_processes() {
	pid_t process_PID;
	int process_status;

	for (int i = 0; i < num_background_processes;) {
		process_PID = waitpid(background_processes[i], &process_status, WNOHANG);

		if (process_PID > 0) { // if waitpid was successful
			printf("background pid %d is done: ", process_PID);

			if (WIFEXITED(process_status)) { // if process exited normally
				printf("exit value %d\n", WEXITSTATUS(process_status));
			} else if (WIFSIGNALED(process_status)) { // if process was signaled to exit
				printf("terminated by signal %d\n", WTERMSIG(process_status));
			}
			fflush(stdout);

			// replace terminated process PID with PID of the last non-complete process in the array
			// this is necessary to avoid gaps in the array
			num_background_processes--;
			background_processes[i] = background_processes[num_background_processes];

			// i is not incremented because the new element at index i needs to be checked
		} else {
			i++; // increment only if process was not complete
		}
	}
}
