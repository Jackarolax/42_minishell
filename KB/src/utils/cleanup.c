/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmonjard <kmonjard@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/06 16:28:06 by kmonjard          #+#    #+#             */
/*   Updated: 2026/05/08 16:13:56 by kmonjard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Cleanup wrapper for next prompt.
 * Clean cmds,
 */
void	cleanup_loop(t_minishell *data)
{
	if (data->input)
	{
		free(data->input);
		data->input = NULL;
	}
	if (data->cmds)
	{
		free_cmds(data->cmds);
		data->cmds = NULL;
	}
	if (data->tokens)
	{
		free_tokens(data->tokens);
		data->tokens = NULL;
	}
}

void	cleanup_shell(t_minishell *data)
{
	cleanup_loop(data);
	if (data->processed_env)
		free_env(data->processed_env);
	if (data->history.buffer)
		free(data->history.buffer);
	if (data->history.history)
		free_str_arrays(data->history.history);
}
