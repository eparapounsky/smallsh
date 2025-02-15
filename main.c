#include <stdio.h> // for printf
#include <string.h> // for strtok_r
#include <stdbool.h> // for true/false
#include <stdlib.h> // for calloc

// macros
#define COMMAND_LINE_SIZE 2048
#define MAX_ARGS 512

// function prototypes
int exit_program();

// structs
struct user_command {
	char* argv[MAX_ARGS +1]; // array holding arguments
	int argc; // number of arguments
	char* input_file;
	char* output_file;
	bool is_background_process;
};

struct user_command* parse_command() {
	char user_input[COMMAND_LINE_SIZE];
	struct user_command* current_command = (struct user_command*) calloc(1, sizeof(struct user_command));

	// get user's command
	printf(": ");
	fflush(stdout); // flush output to display prompt
	fgets(user_input, COMMAND_LINE_SIZE, stdin); // read from standard input

	// parse user's command
	char* token = NULL;
	char* remainder = user_input;

	// strtok_r returns NULL when there are no more tokens
	while ((token = strtok_r(remainder, " \n", &remainder)) != NULL) {
		if(strcmp(token, "<") == 0) { // if user specified input file
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

void free_command_memory(struct user_command* command) {
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

int main () {
	struct user_command* current_command;

	while (true) {
		current_command = parse_command();

		free_command_memory(current_command);
	}
	return 0;
}

int exit_program() {
	return 0;
}
