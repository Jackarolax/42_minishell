#include <signal.h>
#include "minishell.h"

volatile sig_atomic_t g_signal = 0;

/**
 * @brief Ignore CTRL + C
 */
void	signal_handler(int signo)
{
	g_signal = signo;
	if (signo == SIGINT)
	{
		write(1, "\n", 1);
		
	}
}

/**
 *
 */
void	setup_signals(void)
{
	struct sigaction sa;

	sa.sa_handler = signal_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL); // CTRL + C
	sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, NULL); // CTRL + \'
}
