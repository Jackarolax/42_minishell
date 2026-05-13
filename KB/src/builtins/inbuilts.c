/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inbuilts.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anematol <anematol@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 20:50:31 by anematol          #+#    #+#             */
/*   Updated: 2025/12/02 18:39:38 by anematol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <limits.h>
#include <string.h>
#include "../headers/libft.h"
#include "../headers/get_next_line.h"

/**
 * @brief Print to STDOUT, passed string.
 * Checks for -n option.
 */
void	ft_echo(int argc, char **argv)
{
	int no_last_line;
	int i;

	no_last_line = (argv[1] && ft_strcmp(argv[1], "-n") == 0);
	i = no_last_line + 1;
	while (i < argc)
	{
		ft_printf("%s", argv[i]);
		if (argv[i + 1])
			ft_printf(" ");
		i++;
	}
	if (!no_last_line)
		ft_printf("\n");
}

/**
 * @brief Print to STDOUT, "print working directory".
 * i.e. current directory
 */
void	ft_pwd()
{
	char path[PATH_MAX];

	if (getcwd(path, PATH_MAX) == NULL)
		perror("pwd");
	else
		ft_printf("%s\n", path);
}

/**
 * @brief Change directory function expects:
 * 1 argument after `cd` as argv
 * and argc is expected to be 2.
 */
void	ft_cd(int argc, char **argv)
{
	char	path[PATH_MAX];

	if (getcwd(path, PATH_MAX) == NULL)
		return(perror("cd"));
	if (argc < 2)
		return ((void) ft_printf("cd: missing operand\n"));
	if (argc > 2)
		return((void) ft_printf("cd: too many arguments\n"));
	if (argv[1][0] == '/')
	{
		if (chdir(argv[1]) != 0)
			perror("cd");
		return ;
	}
	if (ft_strlen(path) + ft_strlen(argv[1]) + 1 > PATH_MAX)
	return ((void) ft_printf("cd: ENAMETOOLONG\n"));
	ft_strlcat(path, "/", PATH_MAX);
	ft_strlcat(path, argv[1], PATH_MAX);
	if (chdir(path) != 0)
		perror("cd");
}
