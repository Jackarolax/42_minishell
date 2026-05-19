/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmonjard <kmonjard@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/14 16:18:14 by kmonjard          #+#    #+#             */
/*   Updated: 2026/05/14 17:06:51 by kmonjard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Checks if the raw delimiter token contained any quotes.
 */
int	has_quotes(char *str)
{
	while ((*str))
	{
		if ((*str) == '\'' || (*str) == '\"')
			return (1);
		str++;
	}
	return (0);
}

/**
 * @brief Strips bounding quotes from the delimiter without expanding $.
 */
char	*strip_quotes_only(char *str)
{
	char	*res;
	int		i;
	int		q_state;
	int		old_state;

	res = ft_strdup("");
	q_state = 0;
	i = 0;
	while (str[i])
	{
		old_state = q_state;
		update_quote_state(str[i], &q_state);
		if (old_state != q_state)
		{
			i++;
			continue ;
		}
		append_char(&res, str[i++]);
	}
	return (res);
}

/**
 * @brief Expands $ variables in the heredoc body while keeping quotes literal.
 */
char	*expand_heredoc_body(char *str, t_env *env)
{
	char	*res;
	int		i;

	res = ft_strdup("");
	i = 0;
	while (str[i])
	{
		if (str[i] == '$')
			handle_expansion(str, &i, &res, env);
		else
			append_char(&res, str[i++]);
	}
	return (res);
}
