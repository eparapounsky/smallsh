#include <stdio.h> // for printf
#include <string.h> // for strtok_r, strcmp
#include <stdbool.h> // for true/false
#include <stdlib.h> // for calloc
#include "commands.h" // for command handling

// macros
#define COMMAND_LINE_SIZE 2048
#define MAX_ARGS 512

// function prototypes
struct user_command* parse_command();
void free_command_memory(struct user_command* command);

// structs
struct user_command {
	char* argv[MAX_ARGS + 1]; // array holding arguments
	int argc; // number of arguments
	char* input_file;
	char* output_file;
	bool is_background_process;
};

// declare global variables
extern struct user_command* current_command;
