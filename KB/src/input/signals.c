/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmonjard <kmonjard@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 00:04:01 by kmonjard          #+#    #+#             */
/*   Updated: 2026/05/05 00:04:46 by kmonjard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include "minishell.h"

volatile sig_atomic_t g_signal = 0;

/**
 * @brief Ignore CTRL + C
 */
void	signal_handler(int signo)
{
	(void)signo;
	g_signal = 130;
}

/**
 * @brief Initializing signals.
 */
void	setup_signals(void)
{
	struct sigaction sa;

	sa.sa_handler = signal_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, NULL); // CTRL + \'
}
