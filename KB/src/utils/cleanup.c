#include "minishell.h"

/**
 * @brief Free the commands structure except the tokens that are parsed.
 * Do not free those.
 */
void	free_cmds(t_cmd *cmds)
{
	t_cmd	*temp;

	while (cmds)
	{
		temp = cmds->next;
		if (cmds->args)
			free(cmds->args);
		free(cmds);
		cmds = temp;
	}
}

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
 * @brief Cleanup wrapper for next prompt.
 * Clean cmds,
 */
void	cleanup_loop(t_minishell *data)
{
	if (data->input)
	{
		free(data->input);
		data->input = NULL;
	}
	if (data->cmds)
	{
		free_cmds(data->cmds);
		data->cmds = NULL;
	}
	if (data->tokens)
	{
		free_tokens(data->tokens);
		data->tokens = NULL;
	}
}

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

void	free_env_list(t_env_vars *env_list)
{
	t_env_vars *temp;

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

void	cleanup_shell(t_minishell *data)
{
	cleanup_loop(data);
	if (data->processed_env)
		free_env_list(data->processed_env);
	if (data->history.buffer)
		free(data->history.buffer);
	if (data->history.history)
		free_str_arrays(data->history.history);
}
