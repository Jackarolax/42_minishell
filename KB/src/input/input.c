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
 * @brief Listens to STDIN.
 * Checks for CTRL + D
 */
char	*listen_input(int fd, t_minishell *data)
{
	char	*input;
	char	c;
	long	cursor;
	long	input_len;
	ssize_t	n;

	n = 0;
	c = 0;
	cursor = 0;
	input_len = 0;
	input = NULL;
	while (1)
	{
		n = read(fd, &c, 1);
		if (n == -1 || n == 0)
		{
			if (input)
				free(input);
			return (NULL); // error or EOF
		}
		if (c == '\n' || c == '\r')
		{
			write(1, &c, 1);
			break;
		}
		else if (c == 4)
		{
			if (input_len == 0)
			{
				write(1, "exit\n", 5);
				if (input)
					free(input);
				return (NULL);
			}
		}
		else if (c == '\033')
			arrow_keys(data, &input, &cursor, &input_len);
		else if (c == 127)
		{
			if (cursor > 0)
				backspace(&input, &cursor, &input_len);
		}
		else if (ft_isprint(c))
			printable(&input, &c, &cursor, &input_len);
	}
	append_to_history(input, &data->history);
	return (input);
}

