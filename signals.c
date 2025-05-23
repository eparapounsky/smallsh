#include "signals.h"

// global variable to track foreground only mode
bool foreground_commands_only = false;

/**
 * Signal handler for SIGTSTP (CTRL-Z) in the parent shell.
 * SIGTSTP tells the parents process to enable or disable running commands in the foreground only.
 * @param signal_number: int, the integer code representing SIGTSTP
 */
void handle_SIGTSTP (int signal_number) {
	if (foreground_commands_only) {
		write(STDOUT_FILENO, "\nExiting foreground-only mode\n", 30);
		foreground_commands_only = false;
	} else {
		write(STDOUT_FILENO, "\nEntering foreground-only mode (& is now ignored)\n", 50);
		foreground_commands_only = true;
	}
}

/**
 * Registers signal handlers for child processes based on whether they are
 * running in the foreground or background.
 * @param is_background_process: bool, indicates whether the process is a background process
 */
void register_child_signal_handlers(bool is_background_process) {
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

/**
 * Registers signal handlers for the parent shell.
 */
void register_parent_signal_handlers() {
	// initialize structs to be empty
	struct sigaction SIGINT_action = {0};
	struct sigaction SIGTSTP_action = {0};

	// parent process ignores SIGINT
	SIGINT_action.sa_handler = SIG_IGN;
	sigaction(SIGINT, &SIGINT_action, NULL);

	// parent process has a custom handler for SIGTSTP
	SIGTSTP_action.sa_handler = handle_SIGTSTP;
	sigfillset(&SIGTSTP_action.sa_mask); // block all catchable signals while handle_SIGTSTP runs
	SIGTSTP_action.sa_flags = 0; // no flags set
	sigaction(SIGTSTP, &SIGTSTP_action, NULL);
}

/**
 * Getter function for the foreground_commands_only global variable.
 * @return foreground_commands_only: bool, false if both foreground and background commands allowed,
 * true if only foreground commands allowed
 */
bool is_foreground_only_mode() {
	return foreground_commands_only;
}
