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
	char			*input;
	t_minishell		data;

	initialize(argc, argv, envp, &data);
	while (1)
	{
		input = listen_input(STDIN_FILENO);
		if (!input)
			break;
		write(1, input, ft_strlen(input));
		write(1, "\n", 1);
	}
	disable_raw_mode(&data);
	return (g_signal);
}
