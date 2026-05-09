/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmonjard <kmonjard@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 00:05:17 by kmonjard          #+#    #+#             */
/*   Updated: 2026/05/07 21:24:38 by kmonjard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Fills in commands with redirects.
 */
static void	fill_redirs(t_token **tok, t_cmd **cmd)
{
	int	append;

	if ((*tok)->type == TOKEN_REDIR_OUT || (*tok)->type == TOKEN_APPEND)
	{
		if ((*tok)->type == TOKEN_APPEND)
			append = 1;
		else
			append = 0;
		(*cmd)->append = append;
		(*tok) = (*tok)->next;
		if ((*tok))
			(*cmd)->outfile = (*tok)->value;
	}
	else if ((*tok)->type == TOKEN_REDIR_IN)
	{
		(*tok) = (*tok)->next;
		if ((*tok))
			(*cmd)->infile = (*tok)->value;
	}
}

/**
 * @brief Fills in commands, heredocs and calls redir filler.
 */
static void	fill_cmd(t_token **tok, t_cmd **curr, int *i)
{
	if ((*tok)->type == TOKEN_WORD)
	{
		(*curr)->args[(*i)] = (*tok)->value;
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
		fill_redirs(tok, curr);
}

/**
 * @brief Converts each token to command structure for easier parsing later on.
 * Along with their args.
 */
t_cmd	*tokens_to_cmds(t_token *tokens)
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
			fill_cmd(&curr_tok, &curr_cmd, &i);
		if (curr_tok)
			curr_tok = curr_tok->next;
	}
	return (head);
}
