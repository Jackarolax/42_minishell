#include "minishell.h"

/**
 * @brief Appends a new command string to the shell's history array.
 */
void append_to_history(char *input, t_history *history)
{
	long	count;
	char	**new_history;

	if (!input || input[0] == '\0')
		return ;
	count = 0;
	if (history->history)
	{
		while (history->history[count])
			count++;
	}
	history->history_count = count;
	if (count > 0 && ft_strcmp(history->history[count - 1], input) == 0)
		return ;
	new_history = malloc(sizeof(char *) * (count + 2));
	if (!new_history)
		return ;
	count = 0;
	if (history->history)
	{
		while (history->history[count])
		{
			new_history[count] = history->history[count];
			count++;
		}
	}
	new_history[count] = ft_strdup(input);
	new_history[count + 1] = NULL;
	if (history->history)
		free(history->history);
	history->history = new_history;
}
