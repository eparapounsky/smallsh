#include "commands.h"
#include "processes.h"
#include "main.h"
#include "signals.h"

extern int last_exit_status; // from main.c

/**
 * Calls the termination function from processes.c and exits.
 */
void exit_program() {
	terminate_all_processes();
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
 * @param current_command: user_command struct, the current command being executed
 */
void other_commands(struct user_command* current_command) {
	pid_t child_PID = fork(); // spawn child process

	switch (child_PID) {
	case -1: // check if forking failed
		perror("fork() failed\n");
		exit(1);
		break;

	case 0: // child process
		handle_child_process(current_command);
		_exit(0); // terminate child process

	default: // parent process
		handle_parent_process(current_command, child_PID);
	}
	// check for any completed background processes before returning command prompt
	check_background_processes();
}
