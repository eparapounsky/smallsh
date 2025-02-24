#include "signals.h"

// global variables
int foreground_commands_only = 0; // 0 = both foreground and background, 1 = only foreground

/**
 * Signal handler for SIGINT (CTRL-C).
 * The parent process and any children running as background processes ignore SIGINT.
 * A child running as a foreground process terminates itself upon receiving SIGINT (default action).
 * @param signal_number: int, the integer code representing SIGINT
 * Adapted from Module 7: Signal Handling API code
 */
void handle_SIGINT (int signal_number) {
	// empty, because SIGINT is ignored
}

/**
 * Signal handler for SIGTSTP (CTRL-Z).
 * All child processes (background or foreground) ignore SIGTSTP.
 * SIGTSTP tells the parents process to enable or disable running commands in the foreground only.
 * @param signal_number: int, the integer code representing SIGTSTP
 */
void handle_SIGTSTP (int signal_number) {
	if (foreground_commands_only) {
		write(1, "Exiting foreground-only mode\n", 29); // 1 is fd for stdout
		foreground_commands_only = 0;
	} else {
		write(1, "Entering foreground-only mode (& is now ignored)");
		foreground_commands_only = 1;
	}
}

/**
 *
 * Adapted from Module 7: Signal Handling API code
 */
void register_signal_handlers() {
	// SIGINT
	struct sigaction SIGINT_action = {0}; // initialize to be empty
	// fill out the SIGINT_action struct
	SIGINT_action.sa_handler = handle_SIGINT; // register handle_SIGINT as the signal handler
	sigaction(SIGINT, &SIGINT_action, NULL); // install signal handler

	// SIGTSTP
	struct sigaction SIGTSTP_action = {0};
	SIGTSTP_action.sa_handler = handle_SIGTSTP;
	sigaction(SIGTSTP, &SIGTSTP_action, NULL);
}
