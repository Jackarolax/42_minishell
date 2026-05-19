/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmonjard <kmonjard@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 20:31:53 by kmonjard          #+#    #+#             */
/*   Updated: 2026/05/19 02:30:40 by kmonjard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief
 */
static char	*get_tmp_filename(void)
{
	// replace in data, currently placeholder
	static int	counter = 0;

	char		*pid_str;
	char		*cnt_str;
	char		*base;
	char		*res;

	pid_str = ft_itoa(getpid());
	cnt_str = ft_itoa(counter++);
	base = ft_strjoin(".heredoc_tmp_", pid_str);
	free(pid_str);
	pid_str = ft_strjoin(base, "_");
	free(base);
	res = ft_strjoin(pid_str, cnt_str);
	free(pid_str);
	free(cnt_str);
	return (res);
}

/**
 * @brief
 */
static void	read_heredoc(t_redir *redir, t_minishell *data)
{
	int		fd;
	char	*line;
	char	*tmp_file;

	tmp_file = get_tmp_filename();
	fd = open(tmp_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	while (1)
	{
		write(1, "> ", 2);
		line = get_next_line(STDIN_FILENO);
		if (!line || (ft_strncmp(line, redir->file, ft_strlen(redir->file)) == 0
				&& line[ft_strlen(redir->file)] == '\n'))
			break ;
		if (redir->expand_heredoc)
			line = expand_heredoc_body(line, data->processed_env);
		ft_putstr_fd(line, fd);
		free(line);
	}
	if (line)
		free(line);
	close(fd);
	free(redir->file); // Drop old delimiter string
	redir->file = tmp_file; // Assign directly to disk path
}

/**
 * @brief
 */
void	prep_all_heredocs(t_cmd *cmds, t_minishell *data)
{
	t_cmd	*curr_cmd;
	t_redir	*curr_redir;

	curr_cmd = cmds;
	while (curr_cmd)
	{
		curr_redir = curr_cmd->redirs;
		while (curr_redir)
		{
			if (curr_redir->type == HEREDOC)
				read_heredoc(curr_redir, data);
			curr_redir = curr_redir->next;
		}
		curr_cmd = curr_cmd->next;
	}
}
