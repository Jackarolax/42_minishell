/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_and_del.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmonjard <kmonjard@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/09 14:04:37 by kmonjard          #+#    #+#             */
/*   Updated: 2026/05/09 14:06:03 by kmonjard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 *
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
 * @brief
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
