#include "minishell.h"

// Removes the character just BEFORE the given position
char *delete_char(char *str, int pos)
{
	char	*new_str;
	int		len;
	int		i;
	int		j;

	if (!str || pos <= 0)
		return (str);
	len = ft_strlen(str);
	new_str = malloc(sizeof(char) * len);
	if (!new_str)
		return (NULL);
	i = 0;
	j = 0;
	while (str[i])
	{
		if (i == pos - 1) // Skip the character we are deleting
		{
			i++;
			continue;
		}
		new_str[j++] = str[i++];
	}
	new_str[j] = '\0';
	free(str);
	return (new_str);
}

// Inserts character 'c' into 'str' at the index 'pos'
char *insert_char(char *str, char c, int pos)
{
	char	*new_str;
	int		len;
	int		i;
	int		j;

	len = 0;
	if (str != NULL)
		len = ft_strlen(str);
	new_str = malloc(sizeof(char) * (len + 2));
	if (!new_str)
		return (NULL);
	i = 0;
	j = 0;
	while (i <= len)
	{
		if (i == pos)
			new_str[i] = c;
		else
			new_str[i] = str[j++];
		i++;
	}
	new_str[i] = '\0';
	if (str != NULL)
		free(str);
	return (new_str);
}

/**
 * @brief Listens to STDIN.
 * Checks for CTRL + D
 */
char	*listen_input(int fd, t_minishell *data)
{
	char	*input;
	char	c;
	char	seq[3];
	long	cursor;
	long	input_len;
	ssize_t	n;

	n = 0;
	c = 0;
	cursor = 0;
	input_len = 0;
	input = NULL;
	write(1, "~$ ", 3);
	while (1)
	{
		n = read(fd, &c, 1);
		if (n == -1 || n == 0)
		{
			if (input)
				free(input);
			return (NULL);
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
		/**
		 * Arrow keys
		 */
		else if (c == '\033')
		{
			read(fd, &seq[0], 1);
			read(fd, &seq[1], 1);
			if (seq[0] == '[')
			{
				if (seq[1] == 'D')
				{
					if (cursor > 0)
					{
						write(1, "\033[D", 3);
						cursor--;
					}
				}
				else if (seq[1] == 'C')
				{
					if (cursor < input_len)
					{
						write(1, "\033[C", 3);
						cursor++;
					}
				}
				// UP and DOWN here
			}
		}
		/**
		 * Backspace
		 */
		else if (c == 127)
		{
			if (cursor > 0)
			{
				input = delete_char(input, cursor);
				cursor--;
				input_len--;
				write(1, "\b", 1);
				write(1, "\033[K", 3);
				if (cursor < input_len)
				{
					ft_putstr_fd(&input[cursor], 1);
					int moves_back = input_len - cursor;
					while (moves_back-- > 0)
						write(1, "\033[D", 3); // Move Left
				}
			}
		}
		else if (ft_isprint(c))
		{
			input = insert_char(input, c, cursor);
			cursor++;
			input_len++;
			if (cursor == input_len)
				write(1, &c, 1);
			else
			{
				write(1, &c, 1);
				write(1, "\033[K", 3);
				ft_putstr_fd(&input[cursor], 1);
				int moves_back = input_len - cursor;
				while (moves_back-- > 0)
					write(1, "\033[D", 3); // Move Left
			}
		}
	}
	append_to_history(input, data);
	return (input);
}

