/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmonjard <kmonjard@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/06 23:49:03 by kmonjard          #+#    #+#             */
/*   Updated: 2026/05/19 02:12:24 by kmonjard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Counts tokens until the next pipe to size the args array safely.
 */
int	count_args(t_token *curr)
{
	int	count;

	count = 0;
	while (curr && curr->type != PIPE)
	{
		count++;
		curr = curr->next;
	}
	return (count);
}

/**
 * @brief Allocates a new command node and its arguments array.
 * Max args is an int because of argc.
 */
t_cmd	*init_cmd(t_token *curr_start)
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

/**
 * @brief Similar to append to end of list.
 * Appends the node to the back of list.
 */
void	add_cmd_back(t_cmd **list, t_cmd *new_cmd)
{
	t_cmd	*current;

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
