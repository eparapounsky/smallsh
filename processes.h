#ifndef PROCESSES_H
#define PROCESSES_H

#include <sys/types.h> // for pid_t
#include <stdio.h>     // for printf
#include <signal.h>    // for signals
#include <sys/wait.h>  // for WIFEXITED, WIFSIGNALED, WEXITSTATUS, WTERMSIG, waitpid
#include <fcntl.h>     // for open
#include <unistd.h>    // for execvp, dup2, close
#include <stdbool.h>   // for true/false
#include <sys/wait.h>  // for WIFEXITED, WIFSIGNALED, waitpid
#include "main.h"      // for current_command struct

// macros
#define BACKGROUND_PROCESS_LIMIT 200

// function prototypes
void add_background_process(pid_t pid);
void terminate_all_processes();
void check_background_processes();
void handle_child_process(struct user_command *current_command);
void handle_parent_process(struct user_command *current_command, pid_t child_PID);

// declare global variables
extern pid_t background_processes[BACKGROUND_PROCESS_LIMIT];
extern int num_background_processes;

#endif // PROCESSES_H
