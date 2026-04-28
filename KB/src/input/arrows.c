#include "minishell.h"

// static void	up(t_history, char *input, char seq[2], long pos[2])

// /**
//  *
//  */
// static void	up_and_down(t_history *history, char *input, char seq[2], long pos[2])
// {
// 	if (seq[1] == 'A')
// 	{
// 		if (history->history_index > 0)
// 		{
// 			if (history->history_index == history->history_count)
// 			{
// 				if (history->buffer)
// 					free(history->buffer);
// 				if (input)
// 					history->buffer = ft_strdup(input);
// 				else
// 					history->buffer = ft_strdup("");
// 			}
// 			history->history_index--;
// 			write(1, "\r~$ \033[K", 8);
// 			if (history->buffer) free(history->buffer);
// 				history->buffer = ft_strdup(history->history[history->history_index]);
// 			if (history->buffer) ft_putstr_fd(history->buffer, 1);
// 			line_len = ft_strlen(history->buffer);
// 			cursor_pos = line_len;
// 		}
// 	}
// 	else if (seq[1] == 'B')
// 	{

// 	}
// }

/**
 *
 */
void arrow_keys(t_minishell *data, char **input, long *cursor, long *len)
{
	char	seq[2];

	read(STDIN_FILENO, &seq[0], 1);
	read(STDIN_FILENO, &seq[1], 1);
	if (seq[0] == '[')
	{
		if (seq[1] == 'D')
		{
			if ((*cursor) > 0)
			{
				write(1, "\033[D", 3);
				(*cursor)--;
			}
		}
		else if (seq[1] == 'C')
		{
			if ((*cursor) < (*len))
			{
				write(1, "\033[C", 3);
				(*cursor)++;
			}
		}
	}
}
