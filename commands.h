#include <stdio.h> // for printf
#include <unistd.h> // for chdir
#include <stdlib.h> // for getenv
#include <errno.h> // for debugging
#include <string.h> // for debugging


// function prototypes
void exit_program();
void change_directory(char* pathname);
int print_status();
