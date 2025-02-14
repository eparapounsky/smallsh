#include <stdio.h> // for printf

// macros
#define COMMAND_LINE_SIZE 2048

int main () {
	char user_command[COMMAND_LINE_SIZE];
	printf(": ");
	scanf("%s", &user_command);
	return 0;
}
