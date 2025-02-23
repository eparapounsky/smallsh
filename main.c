#include "main.h" // double quotes for local header files
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
	register_signal_handlers(); // set up signal handling

	while (true) {
		current_command = parse_command();

		// print argv for debugging
		printf("\n current command arguments:\n");
		for (int i = 0; current_command->argv[i] != NULL; i++) {
		    printf("argv[%d]: %s\n", i, current_command->argv[i]);
		}
		fflush(stdout);

		// handle built in commands
		if (strcmp(current_command->argv[0], "exit") == 0) {
			exit_program();
		} else if (strcmp(current_command->argv[0], "cd") == 0) {
			change_directory(current_command->argv[1]);
		} else if (strcmp(current_command->argv[0], "status") == 0) {
			print_status();
		} else {
			other_commands(current_command); // handle all non-built in commands
		}

		free_command_memory(current_command);
	}
	return 0;
}

/**
 * Prompts the user for a command and then parses user input into a user_command struct.
 * @return current_command: user_command struct, the current command to be executed
 * Adapted from sample parser code.
 */
struct user_command* parse_command() {
	char user_input[COMMAND_LINE_SIZE];
	struct user_command* current_command = (struct user_command*) calloc(1, sizeof(struct user_command));

	// get user's command
	printf("\n: ");
	fflush(stdout); // flush output to display prompt
	fgets(user_input, COMMAND_LINE_SIZE, stdin); // read from standard input

	// check for blank lines
	if (strlen(user_input) == 0) {
		return 0;
	}

	// parse user's command
	char* token = NULL;
	char* remainder = user_input;

	// strtok_r returns NULL when there are no more tokens
	while ((token = strtok_r(remainder, " \n", &remainder)) != NULL) {
		if (strcmp(token, "#") == 0) { // if user entered a comment
			return 0;
		} else if (strcmp(token, "<") == 0) { // if user specified input file
			// dynamically allocate memory and save file name
			current_command->input_file = strdup(strtok_r(remainder, " \n", &remainder));
		} else if (strcmp(token, ">") == 0) { // if user specified output file
			current_command->output_file = strdup(strtok_r(remainder, " \n", &remainder));
		} else if (strcmp(token, "&") == 0) { // if user specified to run process in background
			current_command->is_background_process = true;
		} else { // user specified an argument
			// add argument to array and increment argument count
			current_command->argv[current_command->argc++] = strdup(token);
		}
	}

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
