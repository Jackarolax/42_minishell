// only do last
#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <termios.h>
#include "libft.h" // contains unistd.h
#include "ft_printf.h"
#include "minishell.h"

extern volatile sig_atomic_t g_signal;

typedef enum s_token_info
{
	ARGUMENT,
	COMMAND,
	BUILT_IN_CD,
	BUILT_IN_ECHO,
	BUILT_IN_PWD,
	BUILT_IN_EXPORT,
	BUILT_IN_UNSET,
	BUILT_IN_ENV,
	BUILT_IN_EXIT,
	PIPE,
	REDIRECT_IN,
	REDIRECT_OUT,
	RERIRECT_OUT_APPEND
}	e_token_info;

/**
 *
 */
typedef struct s_env_vars
{
	char		*key;
	char		**values;
	struct s_env_vars	*next;
}	t_env_vars;

/**
 *
 */
typedef struct s_token
{
	char			*token;
	e_token_info	token_info;
	struct s_token	*next;
}				t_token;

typedef struct s_minishell
{
	char			**history;
	t_env_vars		*processed_env;
	struct termios	orig_settings;
}	t_minishell;

/* raw_mode.c */

void	enable_raw_mode(t_minishell *data);

/* environment.c */

void	add_env_var(t_env_vars *copy, char *key, char *value);

/* inbuilts.c */

void 	ft_echo(char **argv);
void	ft_cd(int argc, char **argv);
void 	ft_pwd();

/* execute.c */

void	perform_tokens(t_list *tokens, t_env_vars **copy);

/* signals.c */

void	setup_signals(void);

/* parsing.c */

char	*listen_input(int fd);
void	initialize(int argc, char **argv, char **envp, t_minishell *data);

/* tokens.c */

t_list	*parse_tokens(char *input);

#endif
