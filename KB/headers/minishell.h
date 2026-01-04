// only do last
#ifndef MINISHELL_H
# define MINISHELL_H

extern volatile sig_atomic_t g_signal;

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "libft.h" // contains unistd.h
#include "ft_printf.h"
#include "minishell.h"

typedef struct s_minishell
{
	char		*key;
	char		*prev;
	struct s_env_var	*next;
}	t_minishell;

typedef struct s_env_vars
{

}	t_env_vars;

/* env_init.c */

void	initialize_env(t_env_vars **env_copy, char **envp);

/* signals.c */

void	setup_signals(void);

/* parsing.c */

void	initialize(int argc, char **argv, t_env_vars *env_copy, char **envp);

#endif
