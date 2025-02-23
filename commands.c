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
	if (WIFEXITED(last_exit_status)) { // if process exited normally
		printf("exit value %d", WEXITSTATUS(last_exit_status));
	} else if (WIFSIGNALED(last_exit_status)) { // if process was signaled to exit
		printf("terminated by signal %d", WTERMSIG(last_exit_status));
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

	char* command = current_command->argv[0];
	char** command_array = current_command->argv;

	switch (child_PID) {
	case -1: // check if forking failed
		perror("fork() failed\n");
		exit(1);
		break;

	case 0: // child process
		if (current_command->is_background_process) {
			register_signal_handlers(); // background processes ignore SIGINT
		}

		// input redirection
		if (current_command->input_file) {
			char* input_file = current_command->input_file;
			int input_file_fd = open(input_file, O_RDONLY);

			if (input_file_fd == -1) { // if opening the file failed
				printf("cannot open %s for input", input_file);
				exit(1);
			}

			// redirect stdin to input file
			int result = dup2(input_file_fd, 0); // 0 is fd for stdin

			if (result == -1) { // if redirection failed
				perror("dup2");
				exit(2);
			}

			close(input_file_fd);
		} else if (current_command->is_background_process) { // background process with no input redirection specified
			int dev_null_fd = open("/dev/null", O_RDONLY);

			// redirect stdin to /dev/null
			int result = dup2(dev_null_fd, 0);

			if (result == -1) { // if redirection failed
				perror("dup2");
				exit(2);
			}

			close(dev_null_fd);
		}

		// output redirection
		if (current_command->output_file) {
			char* output_file = current_command->output_file;
			int output_file_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644); // rw-r--r--

			if (output_file_fd == -1) { // if opening the file failed
				printf("cannot open %s for output", output_file);
				exit(1);
			}

			// redirect stdout to output file
			int result = dup2(output_file_fd, 1); // 1 is fd for stdout

			if (result == -1) { // if redirection failed
				perror("dup2");
				exit(2);
			}

			close(output_file_fd);
		} else if (current_command->is_background_process) { // background process with no output redirection specified
			int dev_null_fd = open("/dev/null", O_RDONLY);

			// redirect stdout to /dev/null
			int result = dup2(dev_null_fd, 1);

			if (result == -1) { // if redirection failed
				perror("dup2");
				exit(2);
			}

			close(dev_null_fd);
		}

		execvp(command, command_array); // replace child with new program (search in PATH variable)

		// if execvp returns, error occurred (shell did not find command to run)
		fprintf(stderr, "%s: command not found", command);
		exit(1);

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
				printf("terminated by signal %d", last_exit_status);
				fflush(stdout);
			}
		} else { // if command runs in background
			if (num_background_processes < BACKGROUND_PROCESS_LIMIT) {
				background_processes[num_background_processes] = child_PID; // add to array of non-completed background processes
				num_background_processes++;

				printf("background pid is %d", child_PID);
				fflush(stdout);
			} else {
				fprintf(stderr, "Limit on background processes has been reached.\n");
			}
		}
	}
	check_background_processes();
}
