#include <stdio.h> // for printf
#include <string.h> // for strtok_r
#include <stdbool.h> // for true/false

// macros
#define COMMAND_LINE_SIZE 2048
#define MAX_ARGS 512

// function prototypes
int exit_program();

// structs
struct user_command {
	char* argv[MAX_ARGS +1];
	int argc;
	char* input_file;
	char* output_file;
	bool is_background;
};

struct user_command* parse_command() {
	char user_input[COMMAND_LINE_SIZE];
	struct user_command* current_command = (struct command_line*) calloc(1, sizeof(struct user_command));
}

int main () {
//	// get user's command
//	char user_command[COMMAND_LINE_SIZE];
//	printf(": ");
//	fgets(user_command, COMMAND_LINE_SIZE, stdin); // read from standard input
//
//	// parse user's command
//	char* token = NULL;
//	char* remainder = user_command;
//
//	// strtok_r returns NULL when there are no more tokens
//	while ((token = strtok_r(remainder, " ", &remainder)) != NULL) {
//		if (strcmp(token, "exit") == 0) {
//			exit_program();
//		}
//	}

	return 0;
}

int exit_program() {
	return 0;
}
