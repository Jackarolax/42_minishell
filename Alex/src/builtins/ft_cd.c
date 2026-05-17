/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anematol <anematol@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/17 14:35:13 by anematol          #+#    #+#             */
/*   Updated: 2026/05/17 15:30:08 by anematol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

static void	cd_arg_error(int argc)
{
	if (argc < 2)
		ft_printf("cd: missing operand\n");
	if (argc > 2)
		ft_printf("cd: too many arguments\n");
}

static void	change_OLDPWD(t_env *env, char *old_path)
{
	t_env	*old_pwd_node;

	old_pwd_node = get_env_node(env, "OLDPWD");
	if (!(old_pwd_node->values))
	{
		(old_pwd_node->values) = malloc(sizeof(char*));
		if (!old_pwd_node->values)
			return ;
	}
	if ((old_pwd_node->values)[0])
		free((old_pwd_node->values)[0]);
	(old_pwd_node->values)[0] = ft_strdup(old_path);
	if (!(old_pwd_node->values)[0])
			return ;
}

void	ft_cd(t_env *env, int argc, char **argv)
{
	char	old_path[PATH_MAX];
	char	path[PATH_MAX];

	if (getcwd(path, PATH_MAX) == NULL || getcwd(old_path, PATH_MAX) == NULL)
		return(perror("cd"));
	if (argc != 2)
		return (cd_arg_error(argc));
	if (argv[1][0] == '/')
	{
		if (chdir(argv[1]) != 0)
			perror("cd");
		change_OLDPWD(env, old_path);
		return ;
	}
	if (ft_strlen(path) + ft_strlen(argv[1]) + 1 > PATH_MAX)
		return ((void) ft_printf("cd: ENAMETOOLONG\n"));
	ft_strlcat(path, "/", PATH_MAX);
	ft_strlcat(path, argv[1], PATH_MAX);
	if (chdir(path) != 0)
		perror("cd");
	change_OLDPWD(env, old_path);
}
