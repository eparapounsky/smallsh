#ifndef REDIRECTION_H
#define REDIRECTION_H

#include <fcntl.h>  // for open
#include <unistd.h> // for dup2, close
#include "main.h"   // for current_command struct

// function prototypes
void handle_input_redirection(struct user_command *current_command);
void handle_output_redirection(struct user_command *current_command);

#endif // REDIRECTION_H
