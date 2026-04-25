#include "minishell.h"

/**
 *
 */
void	free_tokens(t_list *tokens)
{
	ft_lstclear(&tokens, free);
}

/**
 *
 */
void	add_new_token(t_list **tokens, char *str, t_type type)
{
	t_token *new_tok;

	new_tok = malloc(sizeof(t_token));
	if (!new_tok)
		return ;
	new_tok->str = str;
	new_tok->type = type;
	ft_lstadd_back(tokens, ft_lstnew(new_tok));
}

int	handle_quotes()
{

}

/**
 * TODO: fix to norm
 */
t_list	*parse_tokens(char *input)
{
	int		i;
	t_list	*tokens;

	i = 0;
	tokens = NULL;
	while (input[i])
	{
		while(input[i] != '\0' && ft_isspace(input[i]))
			i++;
		if (!input[i])
			break;
		if (input[i] == '\'' || input[i] == '"')
			i = handle_quotes(input, i, &tokens);
		else if (input[i])
			i = handle_quotes()

		// handle redirection operators
		else if ((input[i] == '<' && input[i+1] == '<') || (input[i] == '>' && input[i+1] == '>'))
		{
			char *token = malloc(3);
			token[0] = input[i];
			token[1] = input[i+1];
			token[2] = '\0';
			ft_lstadd_back(&tokens, ft_lstnew(token));
			i += 2;
		}

		// only one redirection
		else if (input[i] == '<' || input[i] == '>')
		{
			char *token = malloc(2);
			token[0] = input[i];
			token[1] = '\0';
			ft_lstadd_back(&tokens, ft_lstnew(token));
			i++;
		}

		// handle normal cmds
		else
		{
			int start = i;
			while (input[i] && input[i] != ' ' && input[i] != '\t' &&
				input[i] != '<' && input[i] != '>' && input[i] != '\'' && input[i] != '"')
				i++;
			int len = i - start;
			char *token = malloc(len + 1);
			ft_strncpy(token, &input[start], len);
			token[len] = '\0';
			ft_lstadd_back(&tokens, ft_lstnew(token));
		}
	}
	return (tokens);
}
