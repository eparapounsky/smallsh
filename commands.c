#include "commands.h"
#include "processes.h"

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
void print_status() {
	// query child termination status
	if (WIFEXITED(last_exit_status)) {
		printf("exit value %d", WEXITSTATUS(last_exit_status));
	} else {
		printf("terminated by signal %d", WTERMSIG(last_exit_status));
	}
	fflush(stdout);
}

/**
 * Handles execution of all commands that are not built in.
 * Adapted from Module 7: Processes and I/O example code.
 */
void other_commands(char* argv[]) {
	pid_t child_PID = fork(); // spawn child process
	int child_status;

	switch (child_PID) {
	case -1:
		perror("fork() failed\n");
		exit(1);
		break;
	case 0:
		// child process
		execvp(current_command->argv[0], current_command->argv); // replace child with new program

		// if execvp returns, error occurred
		fprintf(stderr, "%s: command not found", current_command->argv[0]);
		exit(1);
	default:
		// parent process
		pid_t terminated_child_PID = waitpid(child_PID, &child_status, 0); // get PID of terminated child

	}
}
