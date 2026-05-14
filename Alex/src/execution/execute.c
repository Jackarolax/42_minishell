/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmonjard <kmonjard@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 00:05:20 by kmonjard          #+#    #+#             */
/*   Updated: 2026/05/13 18:39:33 by kmonjard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/wait.h>
#include <sys/stat.h>

/**
 * @brief Parent thread util.
 *
 * Waits for every child. It loops forever until waitpid returns -1
 * meaning, no more child process is there.
 *
 * If the status child is exited properly through return or exit() then
 * grab the signal and save it to g_signal.
 *
 * Else if the child exited using signals, check what signal made it exit
 * Then do 128 + [SIGNUM] to g_signal.
 */
void	wait_all_children(pid_t last_pid)
{
	int	status;

	if (last_pid != -1)
	{
		waitpid(last_pid, &status, 0);
		if (WIFEXITED(status))
			g_signal = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			g_signal = 128 + WTERMSIG(status);
	}
	while (waitpid(-1, &status, 0) > 0)
		;
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
	if (cmd->infile)
		infile(cmd, data);
	if (cmd->outfile)
		outfile(cmd, data);
	if (!cmd->args[0])
		exit(0);
	cmd_path = validate_path(cmd->args[0], data);
	fresh_env = convert_env_to_array(data->processed_env);
	execve(cmd_path, cmd->args, fresh_env);
	perror("execve");
	free_str_arrays(fresh_env);
	free(cmd_path);
	cleanup_shell(data);
	exit(1);
}

void	run_parent(t_cmd *curr, int *prev_fd, int fd[2])
{
	if ((*prev_fd) != -1)
		close((*prev_fd));
	if (curr->next)
	{
		close(fd[1]);
		(*prev_fd) = fd[0];
	}
}

/**
 * @brief Abstract. Execution code.
 *
 * Sets prev_fd to -1. fd for piping. Loops through every command.
 * It checks if there is a new command before setting a pipe up.
 * Then it creates a child process to handle the pipes.
 *
 * For a piped process:
 *
 * - The child process assigns its STOUT to the write of the pipe.
 *
 * - The next child (if it exists) assigns the read of the pipe to its STDIN.
 *
 * The parent manages the pipes and ensures that the prev_fd (read) is
 * available for the next process to assign to its STDIN.
 *
 * More explanations on the redirections inside run_child.
 */
void	execute(t_cmd *cmds, t_minishell *data)
{
	t_cmd	*curr;
	int		fd[2];
	int		prev_fd;
	pid_t	last_pid;

	curr = cmds;
	prev_fd = -1;
	last_pid = -1;
	while (curr)
	{
		if (curr->next)
			pipe(fd);
		data->pid = fork();
		if (data->pid == 0)
			run_child(curr, data, prev_fd, fd);
		else
		{
			last_pid = data->pid;
			run_parent(curr, &prev_fd, fd);
		}
		curr = curr->next;
	}
	wait_all_children(last_pid);
}
