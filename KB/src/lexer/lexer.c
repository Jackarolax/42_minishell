#include "minishell.h"

/**
 * @brief Safely frees the entire token linked list.
 */
static void	free_token_list(t_token *list)
{
	t_token *temp;

	while (list != NULL)
	{
		temp = list->next;
		if (list->value)
			free(list->value);
		free(list);
		list = temp;
	}
}

/**
 * @brief Appends a token node to the end of the token linked list.
 */
static void	add_token_back(t_token **list, t_token *new_node)
{
	t_token *current;

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
static t_token	*new_token(char *value, t_token_type type)
{
	t_token *token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = value;
	token->type = type;
	token->next = NULL;
	return (token);
}

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

static int	get_token_len(char *str)
{
	int i = 0;
	int quote_state = 0;

	while (str[i])
	{
		update_quote_state(str[i], &quote_state);
		if (quote_state == 0 &&
			(str[i] == ' ' || str[i] == '\t' || str[i] == '|' ||
			 str[i] == '<' || str[i] == '>'))
		{
			break;
		}
		i++;
	}
	return (i);
}

// Returns how many characters it consumed (1 or 2)
static int	handle_operator(char *input, int i, t_token **list)
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
 * @brief Parse the input into tokens for processing.
 */
t_token *lexer(char *input)
{
	t_token	*list;
	size_t	i = 0;
	size_t	len = 0;
	char	*word;


	if (!input)
		return (NULL);
	list = NULL;
	while (input[i])
	{
		while (ft_isspace(input[i]))
			i++;
		if (input[i] == '\0')
			break ;
		if (input[i] == '|' || input[i] == '<' || input[i] == '>')
		{
			i += handle_operator(input, i, &list);
		}
		else
		{
			len = get_token_len(&input[i]);
			word = ft_substr(input, i, len);
			add_token_back(&list, new_token(word, TOKEN_WORD));
			i += len;
		}
	}
	return (list);
}
