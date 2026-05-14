/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmonjard <kmonjard@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 00:02:32 by kmonjard          #+#    #+#             */
/*   Updated: 2026/05/13 17:04:25 by kmonjard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Initialize the prompt.
 * Expandable.
 */
void	init_prompt(t_minishell *data)
{
	write_prompt();
	enable_raw_mode(data);
}

/**
 * @brief Might add more features to the prompt string if we can.
 */
void	write_prompt(void)
{
	char	*sig;

	if (g_signal != 0)
	{
		write(1, "shelld0n[", 10);
		sig = ft_itoa(g_signal);
		write(1, sig, ft_strlen(sig));
		free(sig);
		write(1, "] $> ", 6);
	}
	else
		write(1, "shelld0n $> ", 13);
}

/**
 * @brief Prompt ends.
 */
void	end_of_prompt(t_minishell *data)
{
	disable_raw_mode(data);
	append_to_history(&data->input, &data->history);
}
