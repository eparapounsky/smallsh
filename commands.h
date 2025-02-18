#include <stdio.h> // for printf
#include <unistd.h> // for chdir, fork, execvp, dup2
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
