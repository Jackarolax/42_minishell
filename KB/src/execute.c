#include "minishell.h"
#include <sys/wait.h>
#include <sys/types.h>

/**
 * @brief Metacharacter check
 */
int	is_metachar(const char *token)
{
	return (ft_strcmp(token, "|") == 0 ||
			ft_strcmp(token, ">") == 0 ||
			ft_strcmp(token, "<") == 0 ||
			ft_strcmp(token, ">>") == 0 ||
			ft_strcmp(token, "<<") == 0);
}

/**
 * @brief Counts till metacharacter
 */
int count_segment(t_list *cursor) {
	int count = 0;
	while (cursor && !is_metachar(cursor->content)) {
		count++;
		cursor = cursor->next;
	}
	return (count);
}

/**
 * @brief Turns segment count into argv
 */
char **segment_to_argv(t_list *cursor, int count)
{
	char **argv = malloc(sizeof(char *) * (count + 1));
	for (int i = 0; i < count; i++) {
		argv[i] = cursor->content;
		cursor = cursor->next;
	}
	argv[count] = NULL;
	return (argv);
}

/**
 *
 */
e_token_info master_compare(const char *token)
{
	if (ft_strcmp(token, "cd\n") == 0)
		return BUILT_IN_CD;
	else if (ft_strcmp(token, "echo\n") == 0)
		return BUILT_IN_ECHO;
	else if (ft_strcmp(token, "pwd\n") == 0)
		return BUILT_IN_PWD;
	else if (ft_strcmp(token, "export\n") == 0)
		return BUILT_IN_EXPORT;
	else if (ft_strcmp(token, "unset\n") == 0)
		return BUILT_IN_UNSET;
	else if (ft_strcmp(token, "env\n") == 0)
		return BUILT_IN_ENV;
	else if (ft_strcmp(token, "exit\n") == 0)
		return BUILT_IN_EXIT;
	else
		return COMMAND;
}

/**
 *
 */
static void	perform(char *cmd, int argc, char **argv, t_env_vars *env)
{
	int				status;
	pid_t			pid;
	e_token_info	type;

	(void)env;
	type = master_compare(cmd);
	if (type == BUILT_IN_CD)
		ft_cd(argc, argv);
	else if (type == BUILT_IN_ECHO)
		ft_echo(argv);
	else if (type == BUILT_IN_PWD)
		ft_pwd();
	else if (type == COMMAND)
	{
		pid = fork();
		if (pid == 0) // child
		{
			// execve to manually supply path based on subject, try eventually with env.
			execvp(cmd, argv);
			perror("execvp");
			_exit(127);
		}
		else if (pid > 0) // parent
			waitpid(pid, &status, 0);
		else
			perror("fork");
	}
}

/**
 * @brief Perform a token until NULL.
 * Skip over
 */
void	perform_tokens(t_list *tokens, t_env_vars *copy)
{
	char	**argv;
	int		argc;
	t_list *cursor;

	cursor = tokens;
	while (cursor)
	{
		argc = count_segment(cursor);
		if (argc == 0)
		{
			cursor = cursor->next;
			continue;
		}
		argv = segment_to_argv(cursor, argc);
		perform(argv[0], argc, argv, copy);
		for (int i = 0; i < argc; i++)
			cursor = cursor->next;
		if (cursor && is_metachar(cursor->content))
			cursor = cursor->next;
		free(argv);
	}
}
