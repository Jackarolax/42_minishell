#include "minishell.h"

void	write_prompt(void)
{
	write(1, "$> ", 3);
}
