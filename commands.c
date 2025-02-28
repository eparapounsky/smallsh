#include "commands.h"
#include "processes.h"
#include "main.h"
#include "signals.h"

extern int last_exit_status; // from main.c

/**
 * Calls the termination function from processes.c and exits.
 */
void exit_program() {
	terminate_processes();
	exit(0);
}

/**
 * Changes the working directory of the shell to the specified path.
 * @param pathname: string, the name of the directory to change to
 */
void change_directory(char* pathname) {
	char* buffer = pathname;

	// check if no arguments
	if (buffer == NULL) {
		buffer = getenv("HOME"); // set to directory in HOME environment variable
	}

	chdir(buffer);
}

/**
 * Prints out either the exit status or the terminating signal of the last foreground process run by the shell.
 */
void print_status() {
	// query child termination status
	if (WIFEXITED(last_exit_status)) { // if process exited normally
		printf("\nexit value %d\n", WEXITSTATUS(last_exit_status));
	} else if (WIFSIGNALED(last_exit_status)) { // if process was signaled to exit
		printf("\nterminated by signal %d\n", WTERMSIG(last_exit_status));
	}
	fflush(stdout);
}

/**
 * Handles execution of all commands that are not built in.
 * Adapted from Module 7: Processes and I/O example code.
 * @param current_command: user_command struct, the current command being executed
 */
void other_commands(struct user_command* current_command) {
	pid_t child_PID = fork(); // spawn child process
	int child_status;

	switch (child_PID) {
	case -1: // check if forking failed
		perror("fork() failed\n");
		exit(1);
		break;

	case 0: // child process
		handle_child_process(current_command);

		_exit(0); // terminate child process

	default: // parent process
		if (!current_command->is_background_process) { // if command runs in foreground
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
			if (num_background_processes < BACKGROUND_PROCESS_LIMIT) {
				background_processes[num_background_processes] = child_PID; // add to array of non-completed background processes
				num_background_processes++;

				printf("\nbackground pid is %d\n", child_PID);
				fflush(stdout);
			} else {
				fprintf(stderr, "\nlimit on background processes has been reached.\n");
			}
		}
	}
	check_background_processes();
}
