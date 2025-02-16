#include <sys/types.h> // for pid_t
#include <stdio.h>

// macros
#define BACKGROUND_PROCESS_LIMIT 200

// function prototypes
void add_process(pid_t pid);
void terminate_processes();
