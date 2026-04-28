#include "minishell.h"

void	print_history(t_minishell *data)
{
	char	**ptr = data->history;

	while (ptr && *ptr)
	{
		printf("history: %s\n", *ptr);
		ptr++;
	}
}
