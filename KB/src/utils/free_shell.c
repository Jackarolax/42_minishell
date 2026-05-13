/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_shell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmonjard <kmonjard@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/06 16:28:49 by kmonjard          #+#    #+#             */
/*   Updated: 2026/05/13 15:56:38 by kmonjard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Free string arrays.
 */
void	free_str_arrays(char **str)
{
	char	**temp;

	temp = str;
	if (!str)
		return ;
	while (*temp != NULL)
	{
		free(*temp);
		temp++;
	}
	free(str);
}

/**
 * @brief Free the commands structure except the tokens that are parsed.
 * Does not free those.
 */
void	free_cmds(t_cmd *cmds)
{
	t_cmd	*temp;

	while (cmds)
	{
		temp = cmds->next;
		if (cmds->args)
			free_str_arrays(cmds->args);
		if (cmds->infile)
			free(cmds->infile);
		if (cmds->outfile)
			free(cmds->outfile);
		free(cmds);
		cmds = temp;
	}
}

/**
 * @brief Free tokens structure
 */
void	free_tokens(t_token *tokens)
{
	t_token	*temp;

	while (tokens)
	{
		temp = tokens->next;
		if (tokens->value)
			free(tokens->value);
		free(tokens);
		tokens = temp;
	}
}

/**
 * @brief Free env processed list.
 */
void	free_env(t_env *env_list)
{
	t_env	*temp;

	while (env_list)
	{
		temp = env_list->next;
		if (env_list->key)
			free(env_list->key);
		if (env_list->values)
			free_str_arrays(env_list->values);
		free(env_list);
		env_list = temp;
	}
}
