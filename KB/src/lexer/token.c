/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmonjard <kmonjard@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 22:09:11 by kmonjard          #+#    #+#             */
/*   Updated: 2026/05/08 16:19:21 by kmonjard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Appends a token node to the end of the token linked list.
 */
void	add_token_back(t_token **list, t_token *new_node)
{
	t_token	*current;

	if (!list || !new_node)
		return ;
	if (*list == NULL)
	{
		*list = new_node;
		return ;
	}
	current = *list;
	while (current->next != NULL)
	{
		current = current->next;
	}
	current->next = new_node;
}

/**
 * @brief Allocates and initializes a new token node.
 */
t_token	*new_token(char *value, t_token_type type)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = value;
	token->type = type;
	token->next = NULL;
	return (token);
}

/**
 * @brief Return the length of a token that is not a metacharacter.
 */
int	get_token_len(char *str)
{
	int	i;
	int	quote_state;

	i = 0;
	quote_state = 0;
	while (str[i])
	{
		update_quote_state(str[i], &quote_state);
		if (quote_state == 0
			&& (str[i] == ' ' || str[i] == '\t' || str[i] == '|'
				|| str[i] == '<' || str[i] == '>'))
		{
			break ;
		}
		i++;
	}
	return (i);
}
