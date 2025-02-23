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
void register_signal_handlers() {}
