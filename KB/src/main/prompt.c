/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmonjard <kmonjard@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 00:02:32 by kmonjard          #+#    #+#             */
/*   Updated: 2026/05/09 15:25:28 by kmonjard         ###   ########.fr       */
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
 * @brief Might add more features to the prompt string if I can.
 */
void	write_prompt(void)
{
	write(1, "shelld0n $> ", 12);
}

/**
 * @brief Prompt ends.
 */
void	end_of_prompt(t_minishell *data)
{
	disable_raw_mode(data);
	append_to_history(&data->input, &data->history);
}
