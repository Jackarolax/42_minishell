/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmonjard <kmonjard@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 13:30:32 by kmonjard          #+#    #+#             */
/*   Updated: 2026/05/13 13:31:32 by kmonjard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include "minishell.h"

/**
 * @brief Prints the standard Bash syntax error.
 */
static int	syntax_error(char *token_val)
{
	ft_putstr_fd("shelld0n[2]: syntax error near unexpected token `", 2);
	if (token_val)
		ft_putstr_fd(token_val, 2);
	else
		ft_putstr_fd("newline", 2);
	ft_putstr_fd("'\n", 2);
	g_signal = 2;
	return (0);
}

/**
 * @brief Checks the token list for invalid operator placements.
 * Returns 1 if valid, 0 if there is a syntax error.
 */
int	check_syntax(t_token *tokens)
{
	t_token	*curr;

	curr = tokens;
	while (curr)
	{
		if (curr->type == TOKEN_PIPE)
		{
			if (curr == tokens || (curr->next && curr->next->type == TOKEN_PIPE))
				return (syntax_error("|"));
			if (!curr->next)
				return (syntax_error("|"));
		}
		else if (curr->type == TOKEN_REDIR_IN || curr->type == TOKEN_REDIR_OUT ||
				 curr->type == TOKEN_APPEND || curr->type == TOKEN_HEREDOC)
		{
			if (!curr->next)
				return (syntax_error(NULL));
			if (curr->next->type != TOKEN_WORD)
				return (syntax_error(curr->next->value));
		}
		curr = curr->next;
	}
	return (1);
}
