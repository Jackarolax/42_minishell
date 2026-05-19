/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_child.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anematol <anematol@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 17:21:34 by anematol          #+#    #+#             */
/*   Updated: 2026/05/19 17:50:22 by anematol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/wait.h>
#include <sys/stat.h>

static int	check_and_set_flags(t_redir *r, int flags)
{
	if (r->type == APPEND)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	return (flags);
}

static void	apply_redirections_child(t_cmd *cmd, t_minishell *data)
{
	t_redir	*r;
	int		fd;
	int		flags;

	r = cmd->redirs;
	while (r)
	{
		if (r->type == REDIR_IN || r->type == HEREDOC)
			fd = open(r->file, O_RDONLY);
		else
		{
			flags = O_WRONLY | O_CREAT;
			flags = check_and_set_flags(r, flags);
			fd = open(r->file, flags, 0644);
		}
		if (fd < 0)
		{
			perror(r->file);
			cleanup_shell(data);
			exit(1);
		}
		dup2(fd, !(r->type == REDIR_IN || r->type == HEREDOC));
		close(fd);
		r = r->next;
	}
}

/**
 * @brief Validates if the command exists and is not a directory.
 * Handles exits 127 (not found) and 126 (directory).
 */
static char	*validate_path(char *cmd_name, t_minishell *data)
{
	char		*path;
	struct stat	path_stat;

	path = get_cmd_path(cmd_name, data->processed_env);
	if (!path)
	{
		ft_putstr_fd(cmd_name, STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		cleanup_shell(data);
		exit(127);
	}
	if (stat(path, &path_stat) == 0 && S_ISDIR(path_stat.st_mode))
	{
		ft_putstr_fd(cmd_name, STDERR_FILENO);
		ft_putstr_fd(": Is a directory\n", STDERR_FILENO);
		free(path);
		cleanup_shell(data);
		exit(126);
	}
	return (path);
}

static int	run_child_inbuilt(t_cmd *cmd, t_minishell *data)
{
	int	argc;

	argc = 0;
	while (cmd->args[argc])
		argc++;
	if (ft_strcmp(cmd->args[0], "echo") == 0)
		return (ft_echo(argc, cmd->args), 1);
	if (ft_strcmp(cmd->args[0], "pwd") == 0)
		return (ft_pwd(), 1);
	if (ft_strcmp(cmd->args[0], "env") == 0)
		return (ft_env(data->processed_env, argc, cmd->args), 1);
	if (ft_strcmp(cmd->args[0], "export" ) == 0)
		return (ft_export(data->processed_env, argc, cmd->args), 1);
	return (0);
}

/**
 * @brief Child process.
 *
 * The first lines of checking prev_fd and cmd->next are handling pipes.
 *
 * For a redirected process:
 *
 * - When an infile is detected, it opens a file of that string name and
 * it takes its STDIN to be replaced by the input incoming from the in file's fd.
 * Closes that once its done.
 *
 * - When an outfile is detected, it creates a file and uses a ternary to
 * determine whether to append or truncate. Passes 0644 as the default file
 * permissions. Then, it replaces its STDOUT with the out file's fd.
 *
 * In execve, any memory allocated once execve finishes frees the memory.
 * Therefore, the fresh_env is always freed.
 */
void	run_child(t_cmd *cmd, t_minishell *data, int prev_fd, int fd[2])
{
	char	**fresh_env;
	char	*cmd_path;

	handle_pipes(cmd, &prev_fd, fd);
	apply_redirections_child(cmd, data);
	if (!cmd->args[0])
		exit(0);
	if (run_child_inbuilt(cmd, data))
	{
		cleanup_shell(data);
		exit(0);
	}
	cmd_path = validate_path(cmd->args[0], data);
	fresh_env = convert_env_to_array(data->processed_env);
	execve(cmd_path, cmd->args, fresh_env);
	perror("execve");
	free_str_arrays(fresh_env);
	free(cmd_path);
	cleanup_shell(data);
	exit(1);
}
