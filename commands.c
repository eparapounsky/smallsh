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
	// check if no arguments
	if (pathname == NULL) {
		pathname = getenv("HOME"); // set to directory in HOME environment variable
	}

	chdir(pathname);
}

// built in status command
//int print_status() {}
