#include <stdio.h> // for printf

// macros
#define COMMAND_LINE_SIZE 2048

int main () {
	// get user's command
	char user_command[COMMAND_LINE_SIZE];
	printf(": ");

	fgets(user_command, COMMAND_LINE_SIZE, stdin);


	return 0;
}

