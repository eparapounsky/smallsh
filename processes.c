#include "processes.h"
#include "main.h"
#include "signals.h"
#include "redirection.h"

extern int last_exit_status; // from main.c

// define global variables
pid_t background_processes[BACKGROUND_PROCESS_LIMIT]; // array holding PIDs of still incomplete background processes
int num_background_processes = 0;

/**
 * Adds a process's PID to the background process tracking array.
 * @param pid: pid_t, the process's PID
 */
void add_background_process(pid_t pid) {
	if (num_background_processes < BACKGROUND_PROCESS_LIMIT) {
		background_processes[num_background_processes] = pid; // add to array of non-completed background processes
		num_background_processes++;

		printf("\nbackground pid is %d\n", pid);
		fflush(stdout);
	} else {
		fprintf(stderr, "\nlimit on background processes has been reached.\n");
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
			printf("\nbackground pid %d is done: ", process_PID);

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

/**
 *
 * @param current_command: user_command struct, the current command being executed
 */
void handle_child_process(struct user_command* current_command) {
	char* command = current_command->argv[0];
	char** command_array = current_command->argv;

	// determine if the child should run in the background
	bool is_background_process = current_command->is_background_process && !is_foreground_only_mode();

	register_child_signal_handlers(is_background_process);

	handle_input_redirection(current_command);

	handle_output_redirection(current_command);

	// execute the command
	if (command != NULL && command_array != NULL) {
		int status = execvp(command, command_array); // replace child with new program (search in PATH variable)

		if (status == -1) { // if execvp returns, error occurred (shell did not find command to run)
			fprintf(stderr, "\n%s: command not found\n", command);
			exit(1);
		}
	}
}

/**
 *
 * @param current_command: user_command struct, the current command being executed
 * @param child_PID: pid_t, the process ID of the child process
 */
void handle_parent_process(struct user_command* current_command, pid_t child_PID) {
	int child_status;

	if (!current_command->is_background_process) { // if command runs in foreground
		// wait for child to finish
		pid_t terminated_child_PID = waitpid(child_PID, &child_status, 0); // get PID of terminated child

		if (terminated_child_PID == -1) { // check if waiting failed
			perror("waitpid() failed");
		}

		// update last exit status
		if (WIFEXITED(child_status)) { // if child exited normally
			last_exit_status = WEXITSTATUS(child_status);
		} else if (WIFSIGNALED(child_status)) { // if child was signaled to exit
			last_exit_status = WTERMSIG(child_status);
			printf("\nterminated by signal %d\n", last_exit_status);
			fflush(stdout);
		}
	} else { // if command runs in background
		add_background_process(child_PID);
	}
}
