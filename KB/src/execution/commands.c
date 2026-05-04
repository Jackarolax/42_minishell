/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmonjard <kmonjard@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 00:05:17 by kmonjard          #+#    #+#             */
/*   Updated: 2026/05/05 00:05:18 by kmonjard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Counts tokens until the next pipe to size the args array safely.
 */
static int	count_args(t_token *curr)
{
	int count = 0;
	while (curr && curr->type != TOKEN_PIPE)
	{
		count++;
		curr = curr->next;
	}
	return (count);
}

/**
 * @brief Allocates a new command node and its arguments array.
 */
static t_cmd	*init_cmd(t_token *curr_start)
{
	t_cmd	*cmd;
	int		max_args;

	cmd = ft_calloc(1, sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	max_args = count_args(curr_start);
	cmd->args = ft_calloc(max_args + 1, sizeof(char *));
	return (cmd);
}

static void	add_cmd_back(t_cmd **list, t_cmd *new_cmd)
{
	t_cmd *current;

	if (!list || !new_cmd)
		return ;
	if (*list == NULL)
	{
		*list = new_cmd;
		return ;
	}
	current = *list;
	while (current->next != NULL)
		current = current->next;
	current->next = new_cmd;
}

/**
 *
 */
t_cmd	*tokens_to_cmds(t_token *tokens)
{
	t_cmd	*head = NULL;
	t_cmd	*curr_cmd = NULL;
	t_token	*curr_tok = tokens;
	int		i = 0;

	while (curr_tok)
	{
		if (!curr_cmd)
		{
			curr_cmd = init_cmd(curr_tok);
			add_cmd_back(&head, curr_cmd);
			i = 0; // Reset argument index for the new command
		}
		if (curr_tok->type == TOKEN_PIPE)
			curr_cmd = NULL;
		else if (curr_tok->type == TOKEN_REDIR_OUT || curr_tok->type == TOKEN_APPEND)
		{
			curr_cmd->append = (curr_tok->type == TOKEN_APPEND) ? 1 : 0;
			curr_tok = curr_tok->next;
			if (curr_tok)
				curr_cmd->outfile = curr_tok->value;
		}
		else if (curr_tok->type == TOKEN_REDIR_IN)
		{
			curr_tok = curr_tok->next;
			if (curr_tok)
				curr_cmd->infile = curr_tok->value;
		}
		else if (curr_tok->type == TOKEN_HEREDOC)
		{
			curr_cmd->heredoc = 1;
			curr_tok = curr_tok->next;
			if (curr_tok)
				curr_cmd->infile = curr_tok->value;
		}
		else if (curr_tok->type == TOKEN_WORD)
		{
			curr_cmd->args[i] = curr_tok->value;
			i++;
		}
		if (curr_tok)
			curr_tok = curr_tok->next;
	}
	return (head);
}
