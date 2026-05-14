/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmonjard <kmonjard@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 00:05:17 by kmonjard          #+#    #+#             */
/*   Updated: 2026/05/13 18:28:49 by kmonjard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Handles output redirection (> and >>),
 * intermediate file creation,
 * and safely updates pointer states.
 */
static void	handle_redir_out(t_token **tok, t_cmd **cmd, t_minishell *data)
{
	int		fd;
	int		flags;
	char	*file;

	if ((*tok)->type == TOKEN_APPEND)
		(*cmd)->append = 1;
	else
		(*cmd)->append = 0;
	(*tok) = (*tok)->next;
	if (!(*tok))
		return ;
	file = expansion((*tok)->value, data->processed_env);
	if ((*cmd)->outfile)
	{
		if ((*cmd)->append)
			flags = O_CREAT | O_WRONLY | O_APPEND;
		else
			flags = O_CREAT | O_WRONLY | O_TRUNC;
		fd = open((*cmd)->outfile, flags, 0644);
		if (fd >= 0)
			close(fd);
		free((*cmd)->outfile);
	}
	(*cmd)->outfile = file;
}

/**
 * @brief Handles input redirection (<) and
 * frees previously assigned input files.
 */
static void	handle_redir_in(t_token **tok, t_cmd **cmd, t_minishell *data)
{
	char	*file;

	(*tok) = (*tok)->next;
	if (!(*tok))
		return ;
	file = expansion((*tok)->value, data->processed_env);
	if ((*cmd)->infile)
		free((*cmd)->infile);
	(*cmd)->infile = file;
}

/**
 * @brief Fills in commands with redirects,
 * delegating tasks to helpers.
 */
static void	fill_redirs(t_token **tok, t_cmd **cmd, t_minishell *data)
{
	if ((*tok)->type == TOKEN_REDIR_OUT || (*tok)->type == TOKEN_APPEND)
		handle_redir_out(tok, cmd, data);
	else if ((*tok)->type == TOKEN_REDIR_IN)
		handle_redir_in(tok, cmd, data);
}

/**
 * @brief Fills in commands, heredocs and calls redir filler.
 */
static void	fill_cmd(t_token **tok, t_cmd **curr, int *i, t_minishell *data)
{
	if ((*tok)->type == TOKEN_WORD)
	{
		(*curr)->args[(*i)] = expansion((*tok)->value, data->processed_env);
		(*i)++;
	}
	else if ((*tok)->type == TOKEN_HEREDOC)
	{
		(*curr)->heredoc = 1;
		(*tok) = (*tok)->next;
		if ((*tok))
			(*curr)->infile = (*tok)->value;
	}
	else
		fill_redirs(tok, curr, data);
}

/**
 * @brief Converts each token to command structure for easier parsing later on.
 * Along with their args.
 */
t_cmd	*tokens_to_cmds(t_token *tokens, t_minishell *data)
{
	int		i;
	t_cmd	*head;
	t_cmd	*curr_cmd;
	t_token	*curr_tok;

	i = 0;
	head = NULL;
	curr_cmd = NULL;
	curr_tok = tokens;
	while (curr_tok)
	{
		if (!curr_cmd)
		{
			curr_cmd = init_cmd(curr_tok);
			add_cmd_back(&head, curr_cmd);
			i = 0;
		}
		if (curr_tok->type == TOKEN_PIPE)
			curr_cmd = NULL;
		else
			fill_cmd(&curr_tok, &curr_cmd, &i, data);
		if (curr_tok)
			curr_tok = curr_tok->next;
	}
	return (head);
}
