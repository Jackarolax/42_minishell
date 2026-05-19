/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmonjard <kmonjard@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 00:03:44 by kmonjard          #+#    #+#             */
/*   Updated: 2026/05/14 19:06:56 by kmonjard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Reset the index to the count every string entry.
 * Free the buffer if it exists.
 */
void	reset_history(t_history *history)
{
	history->history_index = history->history_count;
	if (history->buffer)
	{
		free(history->buffer);
		history->buffer = NULL;
	}
}

/**
 * @brief Initialize the new history string.
 */
static void	init_new_history(t_history **his, long *c, char **new, char **in)
{
	long	count;

	count = *c;
	if ((*his)->history)
	{
		while ((*his)->history[(*c)])
		{
			new[(*c)] = (*his)->history[(*c)];
			(*c)++;
		}
	}
	new[(*c)] = ft_strdup((*in));
	new[(*c) + 1] = NULL;
}

/**
 * @brief Appends a new command string to the shell's history array.
 */
void	append_to_history(char **input, t_history *history)
{
	long	count;
	char	**new_history;

	if (!(*input) || (*input)[0] == '\0')
		return ;
	count = 0;
	if (history->history)
	{
		while (history->history[count])
			count++;
	}
	history->history_count = count;
	if (count > 0 && ft_strcmp(history->history[count - 1], (*input)) == 0)
		return ;
	new_history = malloc(sizeof(char *) * (count + 2));
	if (!new_history)
		return ;
	count = 0;
	init_new_history(&history, &count, new_history, input);
	if (history->history)
		free(history->history);
	history->history_count = count + 1;
	history->history = new_history;
}
