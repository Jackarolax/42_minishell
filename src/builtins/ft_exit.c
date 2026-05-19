/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anematol <anematol@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/17 14:42:13 by anematol          #+#    #+#             */
/*   Updated: 2026/05/17 17:24:45 by anematol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

void	ft_exit(t_minishell *data_p, int argc, char **argv)
{
	int	exit_code;

	if (argc > 2)
		return ((void) ft_printf("exit: too many arguments\n"));
	if (argc == 1)
		exit_code = 0;
	else
		exit_code = atoi(argv[1]);
	cleanup_loop(data_p);
	cleanup_shell(data_p);
	exit(exit_code);
}
