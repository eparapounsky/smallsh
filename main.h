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

// struct to hold information about user's command
struct user_command {
	char* argv[MAX_ARGS + 1]; // array holding arguments (leave space for null terminator)
	int argc; // number of arguments
	char* input_file;
	char* output_file;
	bool is_background_process;
};

// declare global variable
extern struct user_command* current_command;
