/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_and_del.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmonjard <kmonjard@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/09 14:04:37 by kmonjard          #+#    #+#             */
/*   Updated: 2026/05/09 15:05:18 by kmonjard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Handles characters that are printable. Moves and writes
 * accordingly with cursor and input len.
 */
void	printable(char **input, char *c, long *cursor, long *input_len)
{
	long	moves_back;

	moves_back = 0;
	(*input) = insert_char((*input), (*c), (*cursor));
	(*cursor)++;
	(*input_len)++;
	if ((*cursor) == (*input_len))
		write(1, c, 1);
	else
	{
		write(1, c, 1);
		write(1, "\033[K", 3);
		ft_putstr_fd(&(*input)[(*cursor)], 1);
		moves_back = (*input_len) - (*cursor);
		while (moves_back-- > 0)
			write(1, "\033[D", 3);
	}
}

/**
 * @brief Handles deleting characters. Moves the prompt along
 * the positon to delete the character.
 */
void	backspace(char **input, long *cursor, long *input_len)
{
	long	moves_back;

	moves_back = 0;
	(*input) = delete_char((*input), (*cursor));
	(*cursor)--;
	(*input_len)--;
	write(1, "\b", 1);
	write(1, "\033[K", 3);
	if (cursor < input_len)
	{
		ft_putstr_fd(&(*input)[(*cursor)], 1);
		moves_back = (*input_len) - (*cursor);
		while (moves_back-- > 0)
			write(1, "\033[D", 3);
	}
}
