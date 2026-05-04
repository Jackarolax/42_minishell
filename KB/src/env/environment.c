/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmonjard <kmonjard@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 00:05:33 by kmonjard          #+#    #+#             */
/*   Updated: 2026/05/05 00:05:34 by kmonjard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Takes a char ** of values and stitches them together with ':'
 */
char *stitch_env_values(char **values)
{
	char	*result;
	char	*temp;
	int		i;

	i = 0;
	if (!values || !values[0])
		return (ft_strdup(""));
	result = ft_strdup(values[0]);
	i = 1;
	while (values[i])
	{
		temp = result;
		result = ft_strjoin(result, ":");
		free(temp);
		temp = result;
		result = ft_strjoin(result, values[i]);
		free(temp);
		i++;
	}
	return (result);
}

/**
 * @brief Converts a custom env linked list into a standard char **envp
 */
char	**convert_env_to_array(t_env_vars *env_list)
{
	t_env_vars	*curr = env_list;
	char		**envp;
	int			count = 0;
	int			i = 0;
	char		*stitched_val;
	char		*key_with_equals;

	while (curr)
	{
		count++;
		curr = curr->next;
	}
	envp = ft_calloc(count + 1, sizeof(char *));
	if (!envp)
		return (NULL);
	curr = env_list;
	while (curr)
	{
		stitched_val = stitch_env_values(curr->values);
		key_with_equals = ft_strjoin(curr->key, "=");
		envp[i] = ft_strjoin(key_with_equals, stitched_val);
		free(stitched_val);
		free(key_with_equals);

		i++;
		curr = curr->next;
	}
	return (envp);
}

/**
 * @brief Retrieve node of target key from list.
 */
t_env_vars	*get_env_node(t_env_vars *list, char *target_key)
{
	t_env_vars	*curr;

	curr = list;
	while (curr)
	{
		if (ft_strcmp(curr->key, target_key) == 0)
			return (curr);
		curr = curr->next;
	}
	return (NULL);
}

/**
 * @brief Appends a newly allocated environment variable to the
 * env_vars structure. Will also work for an empty list.
 */
void	add_env_var(t_env_vars *copy, char *key, char *value)
{
	t_env_vars	*curr;
	t_env_vars	*new_node;

	new_node = malloc(sizeof(t_env_vars));
	if (!new_node)
		ft_putstr_fd("Error: ", STDERR_FILENO);
	new_node->key = ft_strdup(key);
	new_node->values = malloc(sizeof(char *) * 2);
	if (!new_node->values)

	new_node->values[0] = ft_strdup(value);
	new_node->values[1] = NULL;
	new_node->next = NULL;
	if (copy == NULL)
	{
		copy = new_node;
		return ;
	}
	curr = copy;
	while (curr->next != NULL)
		curr = curr->next;
	curr->next = new_node;
}
