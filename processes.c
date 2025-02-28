#include "processes.h"
#include "main.h"
#include "signals.h"

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
		printf("\nbackground process limit reached\n");
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
 */
void handle_child_process(struct user_command* current_command) {
	char* command = current_command->argv[0];
	char** command_array = current_command->argv;

	// determine if the child should run in the background
	bool is_background_process = current_command->is_background_process && !is_foreground_only_mode();

	register_child_signal_handlers(is_background_process);

	// input redirection
	if (current_command->input_file) {
		char* input_file = current_command->input_file;
		int input_file_fd = open(input_file, O_RDONLY);

		if (input_file_fd == -1) { // if opening the file failed
			printf("\ncannot open %s for input\n", input_file);
			exit(1);
		}

		// redirect stdin to input file
		int result = dup2(input_file_fd, STDIN_FILENO);

		if (result == -1) { // if redirection failed
			perror("dup2");
			exit(2);
		}

		close(input_file_fd);
	} else if (current_command->is_background_process) { // background process with no input redirection specified
		int dev_null_fd = open("/dev/null", O_RDONLY);

		// redirect stdin to /dev/null
		int result = dup2(dev_null_fd, STDIN_FILENO);

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
			printf("\ncannot open %s for output\n", output_file);
			exit(1);
		}

		// redirect stdout to output file
		int result = dup2(output_file_fd, STDOUT_FILENO);

		if (result == -1) { // if redirection failed
			perror("dup2");
			exit(2);
		}

		close(output_file_fd);
	} else if (current_command->is_background_process) { // background process with no output redirection specified
		int dev_null_fd = open("/dev/null", O_RDONLY);

		// redirect stdout to /dev/null
		int result = dup2(dev_null_fd, STDOUT_FILENO);

		if (result == -1) { // if redirection failed
			perror("dup2");
			exit(2);
		}

		close(dev_null_fd);
	}

	if (command != NULL && command_array != NULL) {
		int status = execvp(command, command_array); // replace child with new program (search in PATH variable)

		if (status == -1) { // if execvp returns, error occurred (shell did not find command to run)
			fprintf(stderr, "\n%s: command not found\n", command);
			exit(1);
		}
	}
}
