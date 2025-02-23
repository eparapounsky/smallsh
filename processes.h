#include <sys/types.h> // for pid_t
#include <stdio.h> // for printf
#include <signal.h> // for signals

// macros
#define BACKGROUND_PROCESS_LIMIT 200

// function prototypes
void add_process(pid_t pid);
void terminate_processes();
void check_background_processes();

// declare global variables
extern pid_t background_processes[BACKGROUND_PROCESS_LIMIT];
extern int num_background_processes;
