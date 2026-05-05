/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmonjard <kmonjard@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 00:05:20 by kmonjard          #+#    #+#             */
/*   Updated: 2026/05/05 14:19:00 by kmonjard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/wait.h>

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
void	wait_all_children(void)
{
	int status;

	while (waitpid(-1, &status, 0) > 0)
	{
		if (WIFEXITED(status))
			g_signal = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			g_signal = 128 + WTERMSIG(status);
	}
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
 *
 */
void	run_child(t_cmd *cmd, t_minishell *data, int prev_fd, int fd[2])
{
	if (prev_fd != -1)
	{
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
	}
	if (cmd->next)
	{
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		close(fd[1]);
	}
	if (cmd->infile)
	{
		int in_fd = open(cmd->infile, O_RDONLY);
		if (in_fd < 0)
		{
			perror(cmd->infile); // error no file
			exit(1);
		}
		dup2(in_fd, STDIN_FILENO);
		close(in_fd);
	}
	if (cmd->outfile)
	{
		int flags = O_WRONLY | O_CREAT | (cmd->append ? O_APPEND : O_TRUNC);
		int out_fd = open(cmd->outfile, flags, 0644);
		if (out_fd < 0)
		{
			perror(cmd->outfile); // "out.txt: Permission denied"
			exit(1);
		}
		dup2(out_fd, STDOUT_FILENO);
		close(out_fd);
	}
	if (!cmd->args[0])
		exit(0);
	char	*cmd_path = get_cmd_path(cmd->args[0], data->processed_env);
	if (!cmd_path)
	{
		// add command not found here
		ft_putstr_fd(": command not found\n", 2);
		exit(127);
	}

	// also, noted that fresh_env is freed anyway after going into execve?
	char **fresh_env = convert_env_to_array(data->processed_env); // somehow this fixes env
	execve(cmd_path, cmd->args, fresh_env);
	perror("execve");
	exit(1);
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
	pid_t	pid;

	curr = cmds;
	prev_fd = -1;
	while (curr)
	{
		if (curr->next)
			pipe(fd);
		pid = fork();
		if (pid == 0)
			run_child(curr, data, prev_fd, fd);
		else
		{
			if (prev_fd != -1)
				close(prev_fd);
			if (curr->next)
			{
				close(fd[1]);
				prev_fd = fd[0];
			}
		}
		curr = curr->next;
	}
	wait_all_children();
}
