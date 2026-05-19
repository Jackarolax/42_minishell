/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmonjard <kmonjard@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/18 17:20:58 by kmonjard          #+#    #+#             */
/*   Updated: 2026/05/18 18:18:10 by kmonjard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 *
 */
t_redir	*new_redir_node(t_token_type type, char *file, int exp)
{
	t_redir	*node;

	node = malloc(sizeof(t_redir));
	if (!node)
		return (NULL);
	node->type = type;
	node->file = file;
	node->expand_heredoc = exp;
	node->next = NULL;
	return (node);
}

/**
 *
 */
void	add_redir_back(t_redir **head, t_redir *new_node)
{
	t_redir	*curr;

	if (!head || !new_node)
		return ;
	if (!*head)
	{
		*head = new_node;
		return ;
	}
	curr = *head;
	while (curr->next)
		curr = curr->next;
	curr->next = new_node;
}

/**
 *
 */
void	free_redirs(t_redir *head)
{
	t_redir	*tmp;

	while (head)
	{
		tmp = head->next;
		if (head->file)
			free(head->file);
		free(head);
		head = tmp;
	}
}
