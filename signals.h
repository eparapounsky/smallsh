#include <signal.h> // for signals
#include <unistd.h> // for write (reentrant)

// function prototypes
void handle_SIGINT (int signal_number);
void register_signal_handlers();
