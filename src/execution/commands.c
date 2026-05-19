/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmonjard <kmonjard@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 00:05:17 by kmonjard          #+#    #+#             */
/*   Updated: 2026/05/19 02:18:02 by kmonjard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Fills in commands with redirects.
 */
static void	fill_redirs(t_token **tok, t_cmd **cmd, t_minishell *data)
{
	t_token_type	type;
	char			*file;

	if ((*tok)->type == REDIR_OUT || (*tok)->type == APPEND
		|| (*tok)->type == REDIR_IN)
	{
		type = (*tok)->type;
		(*tok) = (*tok)->next;
		if (*tok)
		{
			file = expansion((*tok)->value, data->processed_env);
			add_redir_back(&(*cmd)->redirs, new_redir_node(type, file, 0));
		}
	}
}

/**
 * @brief Fills in commands, heredocs and calls redir filler.
 */
static void	fill_cmd(t_token **tok, t_cmd **curr, int *i, t_minishell *data)
{
	char	*delim;
	int		exp;

	if ((*tok)->type == WORD)
	{
		(*curr)->args[(*i)] = expansion((*tok)->value, data->processed_env);
		(*i)++;
	}
	else if ((*tok)->type == HEREDOC)
	{
		(*curr)->heredoc = 1;
		exp = !has_quotes((*tok)->next->value);
		delim = strip_quotes_only((*tok)->next->value);
		add_redir_back(&(*curr)->redirs, new_redir_node(HEREDOC, delim, exp));
		(*tok) = (*tok)->next;
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
		if (curr_tok->type == PIPE)
			curr_cmd = NULL;
		else
			fill_cmd(&curr_tok, &curr_cmd, &i, data);
		if (curr_tok)
			curr_tok = curr_tok->next;
	}
	return (head);
}
