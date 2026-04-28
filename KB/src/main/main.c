/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmonjard <kmonjard@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/24 12:09:49 by kmonjard          #+#    #+#             */
/*   Updated: 2025/12/24 14:09:50 by kmonjard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 *
 */
int main(int argc, char **argv, char **envp)
{
	t_minishell		data;

	initialize(argc, argv, envp, &data);
	while (1)
	{
		write_prompt();
		data.input = listen_input(STDIN_FILENO, &data);
		if (!data.input)
			break ;
		printf("%s\n", data.input);
	}
	disable_raw_mode(&data);
	return (g_signal);
}
