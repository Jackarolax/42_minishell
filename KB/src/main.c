#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "libft.h" // contains unistd.h

/**
 *
 */
void	free_tokens(char **tokens)
{
	int	i;

	i = 0;
	while (tokens[i] != NULL)
	{
		free(tokens[i]);
		i++;
	}
	free(tokens);
}

t_list	*parse_tokens(char *line)
{
	int		i;
	t_list	*tokens;

	tokens = NULL;
	i = 0;
	while (line[i])
	{
		while(ft_isspace(line[i]))
			i++;
		if (!line[i]) // exit if line is end
			break;

		// handle quotes
		if (line[i] == '\'' || line[i] == '"')
		{
			char quote = line[i++];
			int start = i;
			while (line[i] && line[i] != quote)
				i++;
			int len = i - start;
			char *token = malloc(len + 1);
			ft_strncpy(token, &line[start], len);
			token[len] = '\0';
			ft_lstadd_back(&tokens, ft_lstnew(token));
			if (line[i] == quote)
				i++;
		}

		// handle redirection operators
		else if ((line[i] == '<' && line[i+1] == '<') || (line[i] == '>' && line[i+1] == '>'))
		{
			char *token = malloc(3);
			token[0] = line[i];
			token[1] = line[i+1];
			token[2] = '\0';
			ft_lstadd_back(&tokens, ft_lstnew(token));
			i += 2;
		}

		else if (line[i] == '<' || line[i] == '>')
		{
			char *token = malloc(2);
			token[0] = line[i];
			token[1] = '\0';
			ft_lstadd_back(&tokens, ft_lstnew(token));
			i++;
		}

		// handle normal cmds
		else
		{
			int start = i;
			while (line[i] && line[i] != ' ' && line[i] != '\t' &&
				line[i] != '<' && line[i] != '>' && line[i] != '\'' && line[i] != '"')
				i++;
			int len = i - start;
			char *token = malloc(len + 1);
			ft_strncpy(token, &line[start], len);
			token[len] = '\0';
			ft_lstadd_back(&tokens, ft_lstnew(token));
		}
	}
	return (tokens);
}

/**
 *
 */
int main(int argc, char **argv, char **envp)
{
	char	*prompt;
	char	*line;
	t_list	*tokens;

	(void)argc;
	(void)argv;
	(void)envp;
	prompt = "~$ ";
	while (1)
	{
		write(STDOUT_FILENO, prompt, ft_strlen(prompt));
		line = get_next_line(STDIN_FILENO);
		if (!line) // Ctrl + D?
			break;
		if (line[0] == '\n') // for an empty line
		{
			free(line);
			continue;
		}
		tokens = parse_tokens(line);
		free_tokens(tokens);
		free(line);
	}
	printf("exit\n");
	return (0);
}
