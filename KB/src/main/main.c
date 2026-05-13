/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmonjard <kmonjard@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/24 12:09:49 by kmonjard          #+#    #+#             */
/*   Updated: 2025/12/24 14:09:50 by kmonjard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *get_token_name(t_token_type type)
{
	if (type == TOKEN_WORD)
		return ("WORD");
	if (type == TOKEN_PIPE)
		return ("PIPE");
	if (type == TOKEN_REDIR_IN)
		return ("REDIR_IN (<)");
	if (type == TOKEN_REDIR_OUT)
		return ("REDIR_OUT (>)");
	if (type == TOKEN_HEREDOC)
		return ("HEREDOC (<<)");
	if (type == TOKEN_APPEND)
		return ("APPEND (>>)");
	return ("UNKNOWN");
}

void print_tokens(t_token *tokens)
{
	t_token	*curr;
	int		i;

	curr = tokens;
	i = 0;
	printf("\n=== LEXER OUTPUT ===\n");
	if (!curr)
		printf("List is empty!\n");

	while (curr)
	{
		printf("Token %02d | Type: %-15s | Value: [%s]\n", i, get_token_name(curr->type), curr->value);
		curr = curr->next;
		i++;
	}
	printf("====================\n\n");
}

void print_cmds(t_cmd *cmds)
{
	t_cmd	*curr = cmds;
	int		i, c = 0;

	printf("\n=== PARSER OUTPUT ===\n");
	while (curr)
	{
		printf("Command [%d]:\n", c++);

		i = 0;
		while (curr->args[i])
		{
			printf("  Arg[%d]: %s\n", i, curr->args[i]);
			i++;
		}
		if (curr->infile)
			printf("  Infile:  %s (Heredoc: %d)\n", curr->infile, curr->heredoc);
		if (curr->outfile)
			printf("  Outfile: %s (Append: %d)\n", curr->outfile, curr->append);

		curr = curr->next;
		if (curr) printf("      |\n      V\n");
	}
	printf("=====================\n\n");
}

/**
 * @brief Utility to print any NULL-terminated string array.
 * Perfect for debugging your command arguments or tokens.
 */
void print_str_array(char **array, char *name)
{
	int i;

	if (name)
		printf("--- Printing Array: %s ---\n", name);
	if (!array)
	{
		printf("(null array)\n");
		return ;
	}
	i = 0;
	while (array[i])
	{
		printf("[%d]: %s\n", i, array[i]);
		i++;
	}
	printf("--------------------------\n");
}

/**
 * @brief Prints the environment linked list with its 2D value arrays.
 */
void print_env_list(t_env *env)
{
	t_env   *curr;
	int     i;

	curr = env;
	printf("\n========= ENV LINKED LIST =========\n");
	if (!curr)
		printf("Empty List\n");
	while (curr)
	{
		printf("Key: %s\n", curr->key);
		if (curr->values)
		{
			i = 0;
			while (curr->values[i])
			{
				printf("  Value[%d]: %s\n", i, curr->values[i]);
				i++;
			}
		}
		else
			printf("  Value: (NULL)\n");

		curr = curr->next;
		if (curr)
			printf("      |\n      v\n");
	}
	printf("===================================\n\n");
}

/**
 * @brief Main is main.
 */
int main(int argc, char **argv, char **envp)
{
	t_minishell	data;

	initialize(argc, argv, envp, &data);
	while (1)
	{
		init_prompt(&data);
		data.input = listen_input(STDIN_FILENO, &data);
		if (!data.input)
			break ;
		if (!check_input(data.input))
		{
			free(data.input);
			continue ;
		}
		data.tokens = lexer(data.input);
		print_tokens(data.tokens); // to remove
		if (data.tokens && check_syntax(data.tokens))
		{
			data.cmds = tokens_to_cmds(data.tokens);
			print_cmds(data.cmds);  // to remove
			execute(data.cmds, &data);
		}
		cleanup_loop(&data);
	}
	cleanup_shell(&data);
	return (g_signal);
}
