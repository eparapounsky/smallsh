#include <stdio.h> // for printf
#include <string.h> // for strtok_r

// macros
#define COMMAND_LINE_SIZE 2048

int main () {
	// get user's command
	char user_command[COMMAND_LINE_SIZE];
	printf(": ");
	fgets(user_command, COMMAND_LINE_SIZE, stdin); // read from standard input

	// parse user's command
	char* token = NULL;
	char* remainder = user_command;

	// strtok_r returns NULL when there are no more tokens
	while ((token = strtok_r(remainder, " ", &remainder)) != NULL) {
		printf("%s \n", token);
	}


	return 0;
}

