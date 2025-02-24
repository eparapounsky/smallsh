#include <signal.h> // for signals
#include <unistd.h> // for write (reentrant)
#include <stdbool.h> // for true/false

// function prototypes
void handle_SIGINT (int signal_number);
void handle_SIGSTP (int signal_number);
void register_child_signal_handlers(bool is_background_process);
int is_foreground_only_mode();
