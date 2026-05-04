#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <termios.h>
#include <fcntl.h>
#include "libft.h" // contains unistd.h
#include "ft_printf.h"
#include "minishell.h"

extern volatile sig_atomic_t g_signal;

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_HEREDOC,
	TOKEN_APPEND
}	t_token_type;

typedef struct s_cmd
{
	char			**args;
	char			*infile;
	char			*outfile;
	int				append;
	int				heredoc;
	struct s_cmd	*next;
}	t_cmd;

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
	char			*value;
	t_token_type	type;
	struct s_token	*next;
}				t_token;

typedef struct s_history
{
	unsigned long	history_count;
	unsigned long	history_index;
	char			*buffer;
	char			**history;
}	t_history;

/**
 *
 */
typedef struct s_minishell
{
	char			*input;
	t_history		history;
	t_token			*tokens;
	t_cmd			*cmds;
	t_env_vars		*processed_env;
	char			**envp;
	struct termios	orig_settings;
}	t_minishell;

char	**convert_env_to_array(t_env_vars *env_list);
t_env_vars	*get_env_node(t_env_vars *list, char *target_key);

// commands

char	*get_cmd_path(char *cmd, t_env_vars *env_p);

// input_utils.c

int	check_input(char *input);

/* history.c */

void	reset_history(t_history *history);
void	append_to_history(char **input, t_history *history);

// src/input

void	arrow_keys(t_history *history, char **input, long *cursor, long *len);
char	*delete_char(char *str, long pos);
char	*insert_char(char *str, char c, long pos);

// src/main

void	init_prompt(t_minishell *data);

// lexer.c

t_token	*lexer(char *input);
void	update_quote_state(char c, int *quote_state);

// execute.c

void	execute(t_cmd *cmds, t_minishell *data);
t_cmd	*tokens_to_cmds(t_token *tokens);

/* raw_mode.c */

void	enable_raw_mode(t_minishell *data);
void	disable_raw_mode(t_minishell *data);

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

char	*listen_input(int fd, t_minishell *data);
void	initialize(int argc, char **argv, char **envp, t_minishell *data);

/* tokens.c */

t_list	*parse_tokens(char *input);

#endif
