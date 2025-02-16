#include "commands.h"
#include "processes.h"
#include <errno.h>
#include <string.h>

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
	char* homeDir = pathname;
	if (homeDir == NULL) {
		homeDir = getenv("HOME"); // set to directory in HOME environment variable
		printf("HOME: %s\n", homeDir);
	}

	char temp_pathname[512];

	if (getcwd(temp_pathname, sizeof(temp_pathname)) != NULL) {
		printf("before working directory: %s\n", temp_pathname);
	} else {
		printf("Error: %s\n", strerror(errno));
	}

	chdir(homeDir);

	if (getcwd(temp_pathname, sizeof(temp_pathname)) != NULL) {
			printf("after working directory: %s\n", temp_pathname);
		}
}

// built in status command
//int print_status() {}
