/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_resolver.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmonjard <kmonjard@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 00:05:25 by kmonjard          #+#    #+#             */
/*   Updated: 2026/05/05 14:19:03 by kmonjard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Handles the absolute path.
 */
char	*handle_absolute(char *cmd)
{
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, F_OK) == 0 && access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
}

/**
 * @brief Resolves the absolute path of a command.
 * Returns an allocated string with the path, or NULL if not found.
 */
char	*get_cmd_path(char *cmd, t_env_vars *env_p)
{
	t_env_vars	*path_node;
	char		*path;
	char		*part_path;
	int			i;

	path = handle_absolute(cmd);
	if (path)
		return (path);
	path_node = get_env_node(env_p, "PATH");
	if (!path_node || !path_node->values)
		return (NULL);
	i = 0;
	while (path_node->values[i])
	{
		part_path = ft_strjoin(path_node->values[i], "/");
		path = ft_strjoin(part_path, cmd);
		free(part_path);
		if (access(path, F_OK) == 0 && access(path, X_OK) == 0)
			return (path);
		free(path);
		i++;
	}
	return (NULL);
}
