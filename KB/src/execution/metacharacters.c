/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   metacharacters.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmonjard <kmonjard@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/06 17:05:51 by kmonjard          #+#    #+#             */
/*   Updated: 2026/05/13 16:11:45 by kmonjard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Handles when pipes `|` are present
 */
void	handle_pipes(t_cmd *cmd, int *prev_fd, int fd[2])
{
	if ((*prev_fd) != -1)
	{
		dup2((*prev_fd), STDIN_FILENO);
		close((*prev_fd));
	}
	if (cmd->next)
	{
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		close(fd[1]);
	}
}

/**
 * @brief Handles when an infile `<` is present
 */
void	infile(t_cmd *cmd)
{
	int	in_fd;

	in_fd = open(cmd->infile, O_RDONLY);
	if (in_fd < 0)
	{
		perror(cmd->infile);
		exit(1);
	}
	dup2(in_fd, STDIN_FILENO);
	close(in_fd);
}

/**
 * @brief Handles when an outfile `>` is present.
 */
void	outfile(t_cmd *cmd)
{
	int	flags;
	int	out_fd;

	if (cmd->append)
		flags = O_WRONLY | O_CREAT | O_APPEND;
	else
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	out_fd = open(cmd->outfile, flags, 0644);
	if (out_fd < 0)
	{
		perror(cmd->outfile);
		exit(1);
	}
	dup2(out_fd, STDOUT_FILENO);
	close(out_fd);
}
