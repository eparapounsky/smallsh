#include "signals.h"


/**
 * Signal handler for SIGINT (CTRL-C).
 * The parent process and any children running as background processes ignore SIGINT.
 * A child running as a foreground process terminates itself upon receiving SIGINT (default action).
 * @param signal_number: int, the integer code representing SIGINT
 * Adapted from Module 7: Signal Handling API code
 */

void handle_SIGINT (int signal_number) {}

/**
 *
 * Adapted from Module 7: Signal Handling API code
 */
void register_signal_handlers() {
	struct sigaction SIGINT_action = {0}; // initialize to be empty

	// fill out the SIGINT_action struct
	SIGINT_action.sa_handler = handle_SIGINT; // register handle_SIGINT as the signal handler

	sigaction(SIGINT, &SIGINT_action, NULL); // install signal handler
}
