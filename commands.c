#include "commands.h"
#include "processes.h"

/**
 * Calls the termination function from processes.c and exits.
 */
void exit_program() {
	terminate_processes();
	exit(0);
}

// built in cd command
//void change_directory() {}

// built in status command
//int print_status() {}
