#include "commands.h"
#include "processes.h"

// built-in exit command
void exit_program() {
	terimnate_processes();
	exit(0);
}

// built in cd command
void change_directory() {}

// built in status command
int print_status() {}
