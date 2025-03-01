#include "main.h"
#include "commands.h"
#include "processes.h"
#include "signals.h"

// define global variables
int last_exit_status = 0;
struct user_command* current_command = NULL;

/**
 * Main entry point.
 * Prompts the user in a loop for commands, and calls the appropriate handler function.
 */
int main () {
	register_parent_signal_handlers(); // set up signal handling

	while (true) {
		check_background_processes(); // print out finished background processes before taking more user input

		current_command = parse_command();

		if (!current_command) {
			continue; // avoid attempting to process empty commands
		}

		// handle built in commands
		if (strcmp(current_command->argv[0], "exit") == 0) {
			exit_program();
		} else if (strcmp(current_command->argv[0], "cd") == 0) {
			change_directory(current_command->argv[1]);
		} else if (strcmp(current_command->argv[0], "status") == 0) {
			print_status();
		} else {
			// determine if command should be forced to run in foreground
			if (is_foreground_only_mode()) {
				current_command->is_background_process = false;
			}
			other_commands(current_command); // handle all non-built in commands
		}
		free_command_memory(current_command);
	}
	return 0;
}

/**
 * Prompts the user for a command.
 * @return user_input: string, the command entered by the user
 * Adapted from sample parser code.
 */
char* get_command() {
	char* user_input = malloc(COMMAND_LINE_SIZE);

	printf(": ");
	fflush(stdout);
	fgets(user_input, COMMAND_LINE_SIZE, stdin); // read from standard input

	return user_input;
}

/**
 * Parses user input into a user_command struct.
 * @return current_command: user_command struct, the current command to be executed
 * Adapted from sample parser code.
 */
struct user_command* parse_command() {
	char* user_input = get_command();
	struct user_command* current_command = (struct user_command*) calloc(1, sizeof(struct user_command));

	// check for blank lines, comments, and newlines
	if (strlen(user_input) == 0 || user_input[0] == '#' || user_input[0] == '\n') {
		free(user_input);
		free(current_command);
		return NULL;
	}

	// parse user's command
	char* token = NULL;
	char* remainder = user_input;

	// strtok_r returns NULL when there are no more tokens
	while ((token = strtok_r(remainder, " \n", &remainder)) != NULL) {
		 if (strcmp(token, "<") == 0) { // if user specified input file
			current_command->input_file = strdup(strtok_r(remainder, " \n", &remainder)); // dynamically allocate memory and save file name
		} else if (strcmp(token, ">") == 0) { // if user specified output file
			current_command->output_file = strdup(strtok_r(remainder, " \n", &remainder));
		} else if (strcmp(token, "&") == 0) { // if user specified to run process in background
			current_command->is_background_process = true;
		} else { // user specified an argument
			if (current_command->argc < MAX_ARGS) {
				current_command->argv[current_command->argc++] = strdup(token); // add argument to array and increment argument count
			}
		}
	}
	free(user_input);
	return current_command;
}

/**
 * Frees all dynamically allocated memory associated with the given user_command struct.
 * @param command: user_command struct, the struct whose memory is to be freed
 */
void free_command_memory(struct user_command* command) {
    // check that command was created
	if (!command) {
		return;
	}

	// free each argument in argv array
    for (int i = 0; i < command->argc; i++) {
        free(command->argv[i]);
    }

    // free input/output files if needed
    if (command->input_file) {
        free(command->input_file);
    }
    if (command->output_file) {
        free(command->output_file);
    }

    // free the user command struct
    free(command);
}
