#include "minishell.h"

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
