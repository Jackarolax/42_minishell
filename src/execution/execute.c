/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmonjard <kmonjard@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 00:05:20 by kmonjard          #+#    #+#             */
/*   Updated: 2026/05/19 14:35:11 by kmonjard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/wait.h>
#include <sys/stat.h>

void	unlink_heredocs(t_cmd *cmds)
{
	t_cmd	*c;
	t_redir	*r;

	c = cmds;
	while (c)
	{
		r = c->redirs;
		while (r)
		{
			if (r->type == HEREDOC && r->file)
				unlink(r->file);
			r = r->next;
		}
		c = c->next;
	}
}

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
 * @brief Parent function.
 */
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

//if a "parent" inbuilt command gets detected, the function
// will run the command and return 1
// a "parent" inbuilt command does not output to stdout
//will also set cmd = cmd->next if an inbuilt command will be executed
int	run_parent_inbuilt(t_cmd	**cmd_p, t_minishell *data)
{
	int	argc;

	argc = 0;
	while ((*cmd_p)->args[argc])
		argc++;
	if (ft_strcmp((*cmd_p)->args[0], "cd") == 0)
		return (ft_cd(data->processed_env, argc, (*cmd_p)->args),
			*cmd_p = (*cmd_p)->next, 1);
	if (ft_strcmp((*cmd_p)->args[0], "unset") == 0)
		return (ft_unset(&(data->processed_env), argc, (*cmd_p)->args),
			*cmd_p = (*cmd_p)->next, 1);
	if (ft_strcmp((*cmd_p)->args[0], "export" ) == 0 && argc > 1)
		return (ft_export(data->processed_env, argc, (*cmd_p)->args),
			*cmd_p = (*cmd_p)->next, 1);
	if (ft_strcmp((*cmd_p)->args[0], "exit") == 0)
		return (ft_exit(data, argc, (*cmd_p)->args),
			*cmd_p = (*cmd_p)->next, 1);
	return (0);
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
	int		fd[2];
	int		prev_fd;
	pid_t	last_pid;

	prev_fd = -1;
	last_pid = -1;
	prep_all_heredocs(cmds, data);
	while (cmds)
	{
		if (run_parent_inbuilt(&cmds, data))
			continue ;
		if (cmds->next)
			pipe(fd);
		data->pid = fork();
		if (data->pid == 0)
			run_child(cmds, data, prev_fd, fd);
		else
		{
			last_pid = data->pid;
			run_parent(cmds, &prev_fd, fd);
		}
		cmds = cmds->next;
	}
	wait_all_children(last_pid);
	unlink_heredocs(cmds);
}
