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
		write(1, "Entering foreground-only mode (& is now ignored)\n", 49);
		foreground_commands_only = 1;
	}
}

/**
 *
 * Adapted from Module 7: Signal Handling API code
 */
void register_signal_handlers(bool is_background_process) {
	// initialize structs to be empty
	struct sigaction SIGINT_action = {0};
	struct sigaction SIGTSTP_action = {0};

	if (is_background_process) {
		// background processes ignore both SIGINT and SIGTSTP
		// register SIG_IGN (ignore signal) as signal handler for both signals
		SIGINT_action.sa_handler = SIG_IGN;
		SIGTSTP_action.sa_handler = SIG_IGN;
	} else {
		// foreground processes ignore SIGTSTP, but should terminate themselves upon receiving SIGINT (default action)
		// register SIG_DFL (default action) as signal handler for SIGINT
		SIGINT_action.sa_handler = SIG_DFL;
		// register SIG_IGN as signal handler for SIGTSTP
		SIGTSTP_action.sa_handler = SIG_IGN;
	}

	// install the signal handlers
	sigaction(SIGINT, &SIGINT_action, NULL);
	sigaction(SIGTSTP, &SIGTSTP_action, NULL);
}
