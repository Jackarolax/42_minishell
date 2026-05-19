/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmonjard <kmonjard@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 16:45:34 by kmonjard          #+#    #+#             */
/*   Updated: 2026/05/19 02:15:16 by kmonjard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Helper to extract a variable name and fetch its stitched value.
 */
static char	*expand_var(char **str, t_env *env)
{
	char	*var_name;
	t_env	*node;
	int		len;

	(*str)++;
	if (**str == '?')
	{
		(*str)++;
		return (ft_itoa(g_signal));
	}
	len = 0;
	while ((*str)[len] && (ft_isalnum((*str)[len]) || (*str)[len] == '_'))
		len++;
	if (len == 0)
		return (ft_strdup("$"));
	var_name = ft_substr(*str, 0, len);
	(*str) += len;
	node = get_env_node(env, var_name);
	free(var_name);
	if (node && node->values && node->values[0])
		return (stitch_env_values(node->values));
	return (ft_strdup(""));
}

/**
 * @brief Handles expansion and updates the master index and result string.
 * Passes pointers to modify caller's local variables directly.
 */
void	handle_expansion(char *str, int *i, char **res, t_env *env)
{
	char	*ptr;
	char	*expanded;
	char	*temp;

	ptr = &str[(*i)];
	expanded = expand_var(&ptr, env);
	(*i) += (ptr - &str[(*i)]);
	temp = ft_strjoin((*res), expanded);
	free((*res));
	free(expanded);
	(*res) = temp;
}

/**
 * @brief Helper to append a single literal character safely.
 */
void	append_char(char **res, char c)
{
	char	c_str[2];
	char	*temp;

	c_str[0] = c;
	c_str[1] = '\0';
	temp = ft_strjoin((*res), c_str);
	free((*res));
	(*res) = temp;
}

/**
 * @brief Processes a single string token,
 * expanding variables and stripping outer quotes.
 */
char	*expansion(char *str, t_env *env)
{
	char	*res;
	int		i;
	int		quote_state;
	int		old_state;

	res = ft_strdup("");
	quote_state = 0;
	i = 0;
	while (str[i])
	{
		old_state = quote_state;
		update_quote_state(str[i], &quote_state);
		if (old_state != quote_state)
		{
			i++;
			continue ;
		}
		if (str[i] == '$' && quote_state != 1)
			handle_expansion(str, &i, &res, env);
		else
			append_char(&res, str[i++]);
	}
	return (res);
}
