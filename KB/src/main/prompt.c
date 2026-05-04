#include "minishell.h"

void	init_prompt(t_minishell *data)
{
	write(1, "$> ", 3);
	enable_raw_mode(data);
}
