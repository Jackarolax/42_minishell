#include "minishell.h"

/**
 * @brief A subtle check if SHLVL or PWD is not found.
 * This means the shell is level one.
 */
static void	not_found_env(t_env_vars *env_copy)
{
	t_env_vars	*curr;
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
static void	update_variables(t_env_vars *env_copy)
{
	t_env_vars	*curr;
	int			temp;

	curr = env_copy;
	while (curr != NULL)
	{
		if (ft_strcmp(curr->key, "SHLVL") == 0)
		{
			temp = ft_atoi(curr->values[0]) + 1;
			free(curr->values[0]);
			curr->values[0] = ft_itoa(temp);
		}
		else if (ft_strcmp(curr->key, "PWD") == 0)
		{
			free(curr->values[0]);
			curr->values[0] = getcwd(NULL, 0);
		}
		else if (ft_strcmp(curr->key, "OLDPWD") == 0)
		{
			free(curr->values[0]);
			curr->values[0] = NULL;
		}
		curr = curr->next;
	}
	not_found_env(env_copy);
}

/**
 * @brief Initialize a copy of the environment.
 */
static void	initialize_env(t_env_vars *env_copy, char **envp)
{
	int			i;
	t_env_vars	*curr;
	t_env_vars	*prev;

	curr = NULL;
	prev = NULL;
	while (envp && *envp != NULL)
	{
		curr = malloc(sizeof(t_env_vars));
		i = 0;
		while ((*envp)[i] != '=')
			i++;
		curr->key = malloc(sizeof(char) * (i + 1));
		if (!curr->key)
			return ; // add error
		ft_strncpy(curr->key, *envp, i);
		curr->key[i] = '\0';
		curr->values = ft_split(*envp + i + 1, ':'); // check
		if (!curr->values)
			return ;
		curr->next = NULL;
		if (env_copy == NULL)
			env_copy = curr;
		else
			prev->next = curr;
		prev = curr;
		envp++;
	}
	update_variables(env_copy);
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
		ft_putstr_fd("Error: too many arguments\n", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
	ft_bzero(data, sizeof(t_minishell));
	initialize_env(data->processed_env, envp);
	setup_signals();
}

