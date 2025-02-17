#include "commands.h"
#include "processes.h"

/**
 * Calls the termination function from processes.c and exits.
 */
void exit_program() {
	terminate_processes();
	exit(0);
}

/**
 * Changes the working directory of the shell to the specified path.
 */
void change_directory(char* pathname) {
	char* buffer = pathname;

	// check if no arguments
	if (buffer == NULL) {
		buffer = getenv("HOME"); // set to directory in HOME environment variable
	}

//	char temp_pathname[512];
//
//	if (getcwd(temp_pathname, sizeof(temp_pathname)) != NULL) {
//		printf("before changing directory: %s\n", temp_pathname);
//	} else {
//		printf("Error: %s\n", strerror(errno));
//	}

	chdir(buffer);

//	if (getcwd(temp_pathname, sizeof(temp_pathname)) != NULL) {
//			printf("after changing directory: %s\n", temp_pathname);
//	}
}

/**
 * Prints out either the exit status or the terminating signal of the last foreground process run by the shell.
 */

extern int last_exit_status; // from main.h

void print_status() {
	if (WIFEXITED(last_exit_status)) {
		printf("exit value %d", WEXITSTATUS(last_exit_status));
	} else {
		printf("terminated by signal %d", WTERMSIG(last_exit_status));
	}
	fflush(stdout);
}
