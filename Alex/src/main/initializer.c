/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initializer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmonjard <kmonjard@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 00:04:11 by kmonjard          #+#    #+#             */
/*   Updated: 2026/05/13 13:17:06 by kmonjard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief A subtle check if SHLVL or PWD is not found.
 * This means the shell is level one.
 */
static void	not_found_env(t_env *env_copy)
{
	t_env		*curr;
	int			bool_shlvl;
	int			bool_pwd;

	bool_shlvl = 0;
	bool_pwd = 0;
	curr = env_copy;
	while (curr != NULL)
	{
		if (ft_strcmp(curr->key, "SHLVL") == 0)
			bool_shlvl = 1;
		else if (ft_strcmp(curr->key, "PWD") == 0)
			bool_pwd = 1;
		curr = curr->next;
	}
	if (bool_shlvl == 0)
		add_env_var(env_copy, "SHLVL", "1");
	if (bool_pwd == 0)
		add_env_var(env_copy, "PWD", getcwd(NULL, 0));
}

/**
 * @brief Modify built in shell environment variables.
 */
static void	update_variables(t_env *env_copy)
{
	t_env		*curr;

	curr = env_copy;
	while (curr != NULL)
	{
		modify_variables(&curr);
		curr = curr->next;
	}
	not_found_env(env_copy);
}

/**
 * @brief Fills up the current env structure.
 */
static int	fill_env(t_env **env_copy, char **envp, t_env **cur)
{
	size_t	i;

	i = 0;
	while ((*envp)[i] != '=')
		i++;
	(*cur)->key = malloc(sizeof(char) * (i + 1));
	if (!(*cur)->key)
		return (free((*cur)), free_env(*env_copy), 1);
	ft_strncpy((*cur)->key, *envp, i);
	(*cur)->key[i] = '\0';
	(*cur)->values = ft_split(*envp + i + 1, ':');
	if (!(*cur)->values)
		return (free((*cur)->key), free((*cur)), free_env(*env_copy), 1);
	(*cur)->next = NULL;
}

/**
 * @brief Initialize a copy of the environment.
 */
static int	initialize_env(t_env **env_copy, char **envp)
{
	t_env	*curr;
	t_env	*prev;

	curr = NULL;
	prev = NULL;
	while (envp && *envp != NULL)
	{
		curr = malloc(sizeof(t_env));
		if (!curr)
			return (free_env((*env_copy)), 1);
		if (!fill_env(env_copy, envp, &curr))
			return (1);
		if ((*env_copy) == NULL)
			(*env_copy) = curr;
		else
			prev->next = curr;
		prev = curr;
		envp++;
	}
	update_variables((*env_copy));
	return (0);
}

/**
 * @brief Initialize the environment & signal handlers.
 * Exits failure if too many arguments.
 */
void	initialize(int argc, char **argv, char **envp, t_minishell *data)
{
	(void)argv;
	if (argc > 1)
	{
		ft_putstr_fd("shelld0n[1]: too many arguments\n", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
	ft_bzero(data, sizeof(t_minishell));
	initialize_env(&data->processed_env, envp);
	setup_signals();
}
