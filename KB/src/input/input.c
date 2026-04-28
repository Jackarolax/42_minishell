#include "minishell.h"

/**
 *
 */
static void	printable(char **input, char *c, long *cursor, long *input_len)
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
 *
 */
static void	backspace(char **input, long *cursor, long *input_len)
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

/**
 * @brief Manage exits for char c. If c is a newline,
 * write the newline, puts an empty string in input if it is empty
 * and returns 1. Else if c is a EOT (End of Transmission), then exit the shell.
 */
static int	manage_exits(char *c, char **input, long *input_len)
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
		if ((*input_len) == 0)
		{
			write(1, "exit\n", 5);
			if ((*input))
				free((*input));
			return (1);
		}
	}
	return (0);
}

/**
 * @brief Manage character and exits.
 * Checks if the function must exit from manage_exits. If not,
 * check if the char is a key, a backspace or a printable character.
 */
static int	manage_char(char *c, t_minishell *data, long *cursor, long *len)
{
	if (manage_exits(c, &data->input, len))
		return (1);
	else
	{
		if ((*c) == '\033')
			arrow_keys(&data->history, &data->input, cursor, len);
		else if ((*c) == 127)
		{
			if (cursor > 0)
				backspace(&data->input, cursor, len);
		}
		else if (ft_isprint((*c)))
			printable(&data->input, c, cursor, len);
	}
	return (0);
}

/**
 * @brief Listens to STDIN.
 * Checks for CTRL + D
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
			return (NULL); // error or EOF
		}
		if (manage_char(&c, data, &cursor, &input_len))
			break ;
	}
	append_to_history(&data->input, &data->history);
	return (data->input);
}

