/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmonjard <kmonjard@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 00:03:14 by kmonjard          #+#    #+#             */
/*   Updated: 2026/05/19 02:16:39 by kmonjard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <limits.h>
# include <stdio.h>
# include <stdlib.h>
# include <signal.h>
# include <termios.h>
# include <fcntl.h>
# include "libft.h" // contains unistd.h
# include "ft_printf.h"
# include "minishell.h"

extern volatile sig_atomic_t	g_signal;

/**
 * @brief Enumarations for the lexographical part of the program
 */
typedef enum e_token_type
{
	WORD,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	HEREDOC,
	APPEND
}	t_token_type;

/**
 * @brief A linked list for redirections inside the commands.
 *
 * @param type the type of redirection
 * @param file the filename
 * @param expand_heredoc a boolean to determine if the redirections should be
 * expanded
 * @param next the next node
 */
typedef struct s_redir
{
	t_token_type	type;
	char			*file;
	int				expand_heredoc;
	struct s_redir	*next;
}	t_redir;

/**
 * @brief A linked list for every command executed.
 *
 * @param args the argv of the command into execve
 * @param redirs the list of redirections related to the command
 * @param heredoc a boolean to determine if there is a heredoc in parsing
 * @param next the next node
 */
typedef struct s_cmd
{
	char			**args;
	t_redir			*redirs;
	int				heredoc;
	struct s_cmd	*next;
}	t_cmd;

/**
 * @brief A linked list for the environment variables.
 *
 * @param key the key value of an env variable (any string before the equal sign)
 * @param values the values string after
 * the equal sign split by the ':' delimiter, allocated
 * @param next the next node
 */
typedef struct s_env
{
	char				*key;
	char				**values;
	struct s_env		*next;
}	t_env;

/**
 * @brief A linked list for the tokens before converting them into commands.
 * Used for checking bad command tokens, syntax errors, etc.
 *
 * @param value the string of the token
 * @param type the enum type of the token
 * @param next the next node
 */
typedef struct s_token
{
	char			*value;
	t_token_type	type;
	struct s_token	*next;
}				t_token;

/**
 * @brief A structure for storing input strings.
 *
 * @param history_count the number of strings in history.
 * @param history_index the current position of the user in history.
 * @param buffer the input string saved when browsing through history.
 * @param history a string array of inputs parsed by the shell process.
 */
typedef struct s_history
{
	unsigned long	history_count;
	unsigned long	history_index;
	char			*buffer;
	char			**history;
}	t_history;

/**
 * @brief shelld0n data
 *
 * @param input the string inputted from the prompt
 * @param history where history is stored
 * @param tokens where the input is stored after preprocessing
 * @param cmds command structure stored for each command passed
 * @param processed_env an env structure that is dynamic
 * @param pid pid of the current program, set in execution.c
 * @param orig_settings termios strucute of the outer terminal
 */
typedef struct s_minishell
{
	char			*input;
	t_history		history;
	t_token			*tokens;
	t_cmd			*cmds;
	t_env			*processed_env;
	pid_t			pid;
	struct termios	orig_settings;
}	t_minishell;

t_redir	*new_redir_node(t_token_type type, char *file, int exp);

void	add_redir_back(t_redir **head, t_redir *new_node);
void	free_redirs(t_redir *head);
char	*strip_quotes_only(char *str);
char	*expand_heredoc_body(char *str, t_env *env);
void	prep_all_heredocs(t_cmd *cmds, t_minishell *data);
void	unlink_heredocs(t_cmd *cmds);
int		has_quotes(char *str);

void	append_char(char **res, char c);

void	handle_expansion(char *str, int *i, char **res, t_env *env);

void	print_str_array(char **array, char *name);

char	*stitch_env_values(char **values);
int		check_syntax(t_token *tokens);

char	*expansion(char *str, t_env *env);

void	modify_variables(t_env **curr);

void	write_prompt(void);
void	end_of_prompt(t_minishell *data);

void	backspace(char **input, long *cursor, long *input_len);
void	printable(char **input, char *c, long *cursor, long *input_len);

t_token	*new_token(char *value, t_token_type type);
void	add_token_back(t_token **list, t_token *new_node);
int		get_token_len(char *str);

t_cmd	*init_cmd(t_token *curr_start);
void	add_cmd_back(t_cmd **list, t_cmd *new_cmd);

char	**convert_env_to_array(t_env *env_list);
t_env	*get_env_node(t_env *list, char *target_key);
void	cleanup_loop(t_minishell *data);
void	cleanup_shell(t_minishell *data);

void	handle_pipes(t_cmd *cmd, int *prev_fd, int fd[2]);
void	infile(t_cmd *cmd, t_minishell *data);
void	outfile(t_cmd *cmd, t_minishell *data);

void	free_tokens(t_token *tokens);
void	free_env(t_env *env_list);
void	free_cmds(t_cmd *cmds);
void	free_str_arrays(char **str);

void	add_cmd_back(t_cmd **list, t_cmd *new_cmd);
t_cmd	*init_cmd(t_token *curr_start);
int		count_args(t_token *curr);

// commands

char	*get_cmd_path(char *cmd, t_env *env_p);

// input_utils.c

int		check_input(char *input);

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

//run_child.c

void	run_child(t_cmd *cmd, t_minishell *data, int prev_fd, int fd[2]);

// execute.c

void	execute(t_cmd *cmds, t_minishell *data);
t_cmd	*tokens_to_cmds(t_token *tokens, t_minishell *data);

/* raw_mode.c */

void	enable_raw_mode(t_minishell *data);
void	disable_raw_mode(t_minishell *data);

/* environment.c */

void	add_env_var(t_env *copy, char *key, char *value);

/* inbuilts.c */

void	ft_echo(int argc, char **argv);
void	ft_cd(t_env *env, int argc, char **argv);
void	ft_pwd(void);
void	ft_export(t_env *env, int argc, char **argv);
void	ft_unset(t_env **env, int argc, char **argv);
void	ft_env(t_env *env, int argc, char **argv);
void	ft_exit(t_minishell *data);

/* execute.c */

void	perform_tokens(t_list *tokens, t_env **copy);

/* signals.c */

void	setup_signals(void);

/* parsing.c */

char	*listen_input(int fd, t_minishell *data);
void	initialize(int argc, char **argv, char **envp, t_minishell *data);

/* tokens.c */

t_list	*parse_tokens(char *input);

#endif
