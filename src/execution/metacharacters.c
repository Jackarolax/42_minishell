/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   metacharacters.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmonjard <kmonjard@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/06 17:05:51 by kmonjard          #+#    #+#             */
/*   Updated: 2026/05/19 02:12:13 by kmonjard         ###   ########.fr       */
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
