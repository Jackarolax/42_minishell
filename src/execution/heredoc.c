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
static char	*get_tmp_filename(t_minishell *data)
{
	char		*pid_str;
	char		*cnt_str;
	char		*base;
	char		*res;

	pid_str = ft_itoa(getpid());
	cnt_str = ft_itoa(data->heredoc_count++);
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
 * @brief Intercepts keystroke entries dynamically.
 * If ASCII 4 (Ctrl+D) is detected, it handles the terminal cursor adjustment
 * and breaks out cleanly.
 */
char	*get_heredoc_line_raw(void)
{
	char	ch;
	char	buffer[4096];
	int		i;
	ssize_t	ret;

	i = 0;
	ft_bzero(buffer, 4096);
	while (1)
	{
		ret = read(STDIN_FILENO, &ch, 1);
		if (ret < 0)
			return (ft_putstr_fd("shelld0n: read: heredoc\n", 2), NULL);
		if (ch == 4)
			return (NULL);
		if (manage_heredoc_chars(&ch, buffer, &i))
			break ;
	}
	return (ft_strdup(buffer));
}

/**
 * @brief Retrives the line from the raw buffer.
 */
static int	retrieve_line(char **line, t_redir *redir, t_minishell *data)
{
	char	*expanded;

	write(1, "> ", 2);
	(*line) = get_heredoc_line_raw();
	if (!(*line))
	{
		ft_putstr_fd("\nshelld0n: warning: delimited end-of-file (wanted `", 2);
		ft_putstr_fd(redir->file, 2);
		ft_putstr_fd("')\n", 2);
		return (1);
	}
	if (!(*line)
		|| (ft_strncmp((*line), redir->file, ft_strlen(redir->file)) == 0
			&& (*line)[ft_strlen(redir->file)] == '\n'))
		return (1);
	if (redir->expand_heredoc)
	{
		expanded = expand_heredoc_body((*line), data->processed_env);
		free((*line));
		(*line) = expanded;
	}
	return (0);
}

/**
 * @brief
 */
static void	read_heredoc(t_redir *redir, t_minishell *data)
{
	int		fd;
	char	*line;
	char	*tmp_file;

	tmp_file = get_tmp_filename(data);
	fd = open(tmp_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	enable_raw_mode(data);
	while (1)
	{
		if (retrieve_line(&line, redir, data))
			break ;
		ft_putstr_fd(line, fd);
		free(line);
	}
	if (line)
		free(line);
	disable_raw_mode(data);
	close(fd);
	free(redir->file);
	redir->file = tmp_file;
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
