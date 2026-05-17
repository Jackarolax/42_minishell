/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anematol <anematol@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/17 14:40:10 by anematol          #+#    #+#             */
/*   Updated: 2026/05/17 14:40:52 by anematol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "libft.h"
#include "minishell.h"

//will return the start of the list
//assumes there are more than 1 environment variables
//will do nothing if node_to_delete is NULL
t_env	*delete_node(t_env *env_start, t_env *node_to_delete)
{
	t_env	*previous_node;
	t_env	*next_node;

	if (!node_to_delete)
		return (env_start);
	next_node = node_to_delete->next;
	if (node_to_delete == env_start)
	{
		free(node_to_delete->key);
		free_str_arrays(node_to_delete->values);
		free(node_to_delete);
		return (next_node);
	}
	previous_node = env_start;
	while (previous_node && previous_node->next != node_to_delete)
		previous_node = previous_node->next;
	previous_node->next = next_node;
	free(node_to_delete->key);
	free_str_arrays(node_to_delete->values);
	free(node_to_delete);
	return (env_start);
}

void ft_unset(t_env **env_p, int argc, char **argv)
{
	int		i;

	i = 1;
	while (i < argc)
	{
		*env_p = delete_node(*env_p, get_env_node(*env_p, argv[i]));
		i++;
	}
}
