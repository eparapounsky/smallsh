#include "redirection.h"
#include "main.h"

/**
 * Handles input redirection for a child process.
 * Redirects input to a specified file if given.
 * If a process has no specified input file, and runs in the background, input is redirected to /dev/null.
 * @param current_command: user_command struct, the current command being executed
 */
void handle_input_redirection(struct user_command* current_command) {
	if (current_command->input_file) {
		char* input_file = current_command->input_file;
		int input_file_fd = open(input_file, O_RDONLY);

		// check if opening the file failed
		if (input_file_fd == -1) {
			printf("\ncannot open %s for input\n", input_file);
			exit(1);
		}

		// redirect stdin to input file
		int result = dup2(input_file_fd, STDIN_FILENO);

		if (result == -1) {
			perror("dup2");
			exit(2);
		}
		close(input_file_fd);
	} else if (current_command->is_background_process) { // background process with no input redirection specified
		int dev_null_fd = open("/dev/null", O_RDONLY);

		// redirect stdin to /dev/null
		int result = dup2(dev_null_fd, STDIN_FILENO);

		// check if redirection failed
		if (result == -1) {
			perror("dup2");
			exit(2);
		}
		close(dev_null_fd);
	}
}

/**
 * Handles output redirection for a child process.
 * Redirects output to a specified file if given.
 * If a process has no specified output file, and runs in the background, output is redirected to /dev/null.
 * @param current_command: user_command struct, the current command being executed
 */
void handle_output_redirection(struct user_command* current_command) {
	if (current_command->output_file) {
		char* output_file = current_command->output_file;
		int output_file_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644); // rw-r--r--

		// check if opening the file failed
		if (output_file_fd == -1) {
			printf("\ncannot open %s for output\n", output_file);
			exit(1);
		}

		// redirect stdout to output file
		int result = dup2(output_file_fd, STDOUT_FILENO);

		// check if redirection failed
		if (result == -1) {
			perror("dup2");
			exit(2);
		}
		close(output_file_fd);
	} else if (current_command->is_background_process) { // background process with no output redirection specified
		int dev_null_fd = open("/dev/null", O_RDONLY);

		// redirect stdout to /dev/null
		int result = dup2(dev_null_fd, STDOUT_FILENO);

		// if redirection failed
		if (result == -1) {
			perror("dup2");
			exit(2);
		}
		close(dev_null_fd);
	}
}
