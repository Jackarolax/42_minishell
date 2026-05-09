/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmonjard <kmonjard@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 00:03:56 by kmonjard          #+#    #+#             */
/*   Updated: 2026/05/09 14:51:51 by kmonjard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*end_of_text(t_minishell *data)
{
	write(1, "^C\n", 3);
	g_signal = 130;
	if (data->history.buffer)
	{
		free(data->history.buffer);
		data->history.buffer = NULL;
	}
	return (ft_strdup(""));
}

/**
 * @brief Wrapper to handle CTRCTRLC
 */
static int	ctrl_c(char **input, t_minishell *data)
{
	if (data->input)
		free(data->input);
	(*input) = end_of_text(data);
	return (1);
}

/**
 * @brief Manage exits for char c. If c is a newline,
 * write the newline, puts an empty string in input if it is empty
 * and returns 1. Else if c is a EOT (End of Transmission), then exit the shell.
 */
static int	manage_exits(char *c, char **input, long *len, t_minishell *data)
{
	if ((*c) == '\n' || (*c) == '\r')
	{
		write(1, "\n", 1);
		if ((*input) == NULL)
			(*input) = ft_strdup("");
		return (1);
	}
	else if ((*c) == 4)
	{
		if ((*len) == 0)
		{
			write(1, "\nexit\n", 6);
			if ((*input))
			{
				free((*input));
				(*input) = NULL;
			}
			return (1);
		}
	}
	else if ((*c) == 3)
		return (ctrl_c(input, data));
	return (0);
}

/**
 * @brief Manage character and exits.
 * Checks if the function must exit from manage_exits. If not,
 * check if the char is a key, a backspace or a printable character.
 */
static int	manage_char(char *c, t_minishell *data, long *cursor, long *len)
{
	if (manage_exits(c, &data->input, len, data))
		return (1);
	else
	{
		if ((*c) == '\033')
			arrow_keys(&data->history, &data->input, cursor, len);
		else if ((*c) == 127)
		{
			if ((*cursor) > 0)
				backspace(&data->input, cursor, len);
		}
		else if (ft_isprint((*c)))
			printable(&data->input, c, cursor, len);
	}
	return (0);
}

/**
 * @brief Listens to STDIN.
 * Checks for CTRL + D,
 */
char	*listen_input(int fd, t_minishell *data)
{
	char	c;
	long	cursor;
	long	input_len;
	ssize_t	n;

	n = 0;
	c = 0;
	cursor = 0;
	input_len = 0;
	data->input = NULL;
	reset_history(&data->history);
	while (1)
	{
		n = read(fd, &c, 1);
		if (n == -1 || n == 0)
		{
			if (data->input)
				free(data->input);
			return (NULL);
		}
		if (manage_char(&c, data, &cursor, &input_len))
			break ;
	}
	end_of_prompt(data);
	return (data->input);
}
