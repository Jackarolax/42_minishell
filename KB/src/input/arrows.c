/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arrows.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmonjard <kmonjard@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 00:03:14 by kmonjard          #+#    #+#             */
/*   Updated: 2026/05/09 14:18:36 by kmonjard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Up key. If the index is currently greater than 0, then check.
 * If we are at the last history index, then let's save the input into buffer.
 * Then subtract the index.
 */
static void	up(t_history *history, char **input, long *cursor, long *line_len)
{
	if (history->history_index > 0)
	{
		if (history->history_index == history->history_count)
		{
			if (history->buffer)
				free(history->buffer);
			if ((*input))
				history->buffer = ft_strdup((*input));
			else
				history->buffer = ft_strdup("");
		}
		history->history_index--;
		write(1, "\r", 1);
		write_prompt();
		write(1, "\033[K", 3);
		if ((*input))
			free((*input));
		(*input) = ft_strdup(history->history[history->history_index]);
		if ((*input))
			ft_putstr_fd((*input), 1);
		(*line_len) = ft_strlen((*input));
		(*cursor) = (*line_len);
	}
}

/**
 * @brief Down key. If the current index is less than the count,
 * increment, and then free the current input. If we are at 0,
 * duplicate the buffer into input. Else, retrieve the appropriate string.
 */
static void	down(t_history *history, char **input, long *cursor, long *line_len)
{
	if (history->history_index < history->history_count)
	{
		history->history_index++;
		write(1, "\r", 1);
		write_prompt();
		write(1, "\033[K", 3);
		if ((*input))
			free((*input));
		if (history->history_index == history->history_count)
		{
			if (history->buffer)
				(*input) = ft_strdup(history->buffer);
			else
				(*input) = ft_strdup("");
		}
		else
			(*input) = ft_strdup(history->history[history->history_index]);
		if (*input)
			ft_putstr_fd((*input), 1);
		(*line_len) = ft_strlen((*input));
		(*cursor) = (*line_len);
	}
}

/**
 * @brief Left key. Write the left key appropriately.
 */
static void	left(long *cursor)
{
	if ((*cursor) > 0)
	{
		write(1, "\033[D", 3);
		(*cursor)--;
	}
}

/**
 * @brief Right key. Write the right key appropriately.
 */
static void	right(long *cursor, long *len)
{
	if ((*cursor) < (*len))
	{
		write(1, "\033[C", 3);
		(*cursor)++;
	}
}

/**
 * @brief Manages all arrow key inputs. Up, down, left and right keys.
 */
void	arrow_keys(t_history *history, char **input, long *cursor, long *len)
{
	char	seq[2];

	read(STDIN_FILENO, &seq[0], 1);
	read(STDIN_FILENO, &seq[1], 1);
	if (seq[0] == '[')
	{
		if (seq[1] == 'D')
			left(cursor);
		else if (seq[1] == 'C')
			right(cursor, len);
		else if (seq[1] == 'A')
			up(history, input, cursor, len);
		else if (seq[1] == 'B')
			down(history, input, cursor, len);
	}
}
