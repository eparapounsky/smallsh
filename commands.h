#include <stdio.h> // for printf
#include <unistd.h> // for chdir
#include <stdlib.h> // for getenv
#include <errno.h> // for debugging
#include <string.h> // for debugging
#include <sys/wait.h> // for WIFEXITED and wIFSIGNALED


// function prototypes
void exit_program();
void change_directory(char* pathname);
void print_status();
