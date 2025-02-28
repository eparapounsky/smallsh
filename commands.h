#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdio.h> // for printf
#include <unistd.h> // for chdir, fork, execvp, dup2, close
#include <stdlib.h> // for getenv
#include <errno.h> // for debugging
#include <string.h> // for debugging
#include <sys/wait.h> // for WIFEXITED, WIFSIGNALED, waitpid
#include <fcntl.h> // for open

// function prototypes
void exit_program();
void change_directory(char* pathname);
void print_status();
void other_commands();

#endif // COMMANDS_H
