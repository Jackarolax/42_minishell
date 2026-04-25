#include "minishell.h"



/**
 * @brief Print out err in stdout.
 */
void	exit_err(char *err)
{
	ft_putstr_fd(err, STDERR_FILENO);
}
