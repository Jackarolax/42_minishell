/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anematol <anematol@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/17 14:35:30 by anematol          #+#    #+#             */
/*   Updated: 2026/05/17 14:59:22 by anematol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

void	ft_echo(int argc, char **argv)
{
	int	no_last_line;
	int	i;

	// Check for -n option
	no_last_line = (argv[1] && ft_strcmp(argv[1], "-n") == 0);
	i = no_last_line + 1;
	// Print the arguments
	while (i < argc)
	{
		ft_printf("%s", argv[i]);
		if (argv[i + 1])
			ft_printf(" ");
		i++;
	}
	// Print newline if -n option is not specified
	if (!no_last_line)
		ft_printf("\n");
}
