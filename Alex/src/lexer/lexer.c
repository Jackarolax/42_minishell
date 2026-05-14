/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmonjard <kmonjard@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 00:02:58 by kmonjard          #+#    #+#             */
/*   Updated: 2026/05/09 15:08:42 by kmonjard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Updates the quote state integer based on the current character.
 * 0 = No quotes, 1 = Single quotes, 2 = Double quotes.
 */
void	update_quote_state(char c, int *quote_state)
{
	if (c == '\'' && *quote_state == 0)
		*quote_state = 1;
	else if (c == '\'' && *quote_state == 1)
		*quote_state = 0;
	else if (c == '\"' && *quote_state == 0)
		*quote_state = 2;
	else if (c == '\"' && *quote_state == 2)
		*quote_state = 0;
}

/**
 * @brief Handles redirects that have two characters.
 */
static int	handle_double_ops(char *input, int i, t_token **list)
{
	if (input[i] == '<' && input[i + 1] == '<')
	{
		add_token_back(list, new_token(ft_strdup("<<"), TOKEN_HEREDOC));
		return (2);
	}
	else if (input[i] == '>' && input[i + 1] == '>')
	{
		add_token_back(list, new_token(ft_strdup(">>"), TOKEN_APPEND));
		return (2);
	}
}

/**
 * @brief Handle quotes and double quotes and pipes.
 * Returns how many characters it consumed (1 or 2)
 */
static int	handle_operator(char *input, int i, t_token **list)
{
	if ((input[i] == '<' && input[i + 1] == '<')
		|| ((input[i] == '>' && input[i + 1] == '>')))
		return (handle_double_ops(input, i, list));
	else if (input[i] == '<')
	{
		add_token_back(list, new_token(ft_strdup("<"), TOKEN_REDIR_IN));
		return (1);
	}
	else if (input[i] == '>')
	{
		add_token_back(list, new_token(ft_strdup(">"), TOKEN_REDIR_OUT));
		return (1);
	}
	else if (input[i] == '|')
	{
		add_token_back(list, new_token(ft_strdup("|"), TOKEN_PIPE));
		return (1);
	}
	return (0);
}

/**
 * @brief Initializes the token of a word and appends it on the list.
 */
static size_t	init_token(char *input, char **word, size_t *i, t_token **list)
{
	size_t	len;

	len = 0;
	len = get_token_len(&input[(*i)]);
	(*word) = ft_substr(input, (*i), len);
	add_token_back(&(*list), new_token((*word), TOKEN_WORD));
	return (len);
}

/**
 * @brief Parse the input into tokens for processing.
 */
t_token	*lexer(char *input)
{
	t_token	*list;
	size_t	i;
	char	*word;

	if (!input)
		return (NULL);
	list = NULL;
	i = 0;
	word = NULL;
	while (input[i])
	{
		while (ft_isspace(input[i]))
			i++;
		if (input[i] == '\0')
			break ;
		if (input[i] == '|' || input[i] == '<' || input[i] == '>')
			i += handle_operator(input, i, &list);
		else
			i += init_token(input, &word, &i, &list);
	}
	return (list);
}
