/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anematol <anematol@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/17 14:35:13 by anematol          #+#    #+#             */
/*   Updated: 2026/05/19 14:48:25 by anematol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

/**
 *
 */
static void	cd_arg_error(int argc)
{
	if (argc < 2)
		ft_printf("cd: missing operand\n");
	if (argc > 2)
		ft_printf("cd: too many arguments\n");
}

/**
 *
 */
static void	change_oldpwd(t_env *env, char *old_path)
{
	t_env	*old_pwd_node;
	const char	*export_argv[2];

	export_argv[0] = "export";
	export_argv[1] = "OLDPWD";
	old_pwd_node = get_env_node(env, "OLDPWD");
	if (!old_pwd_node)
	{
		ft_export(env, 2, (char **) export_argv);
		old_pwd_node = get_env_node(env, "OLDPWD");
	}
	if (!(old_pwd_node->values))
	{
		(old_pwd_node->values) = malloc(sizeof(char *));
		if (!old_pwd_node->values)
			return ;
	}
	if ((old_pwd_node->values)[0])
		free((old_pwd_node->values)[0]);
	(old_pwd_node->values)[0] = ft_strdup(old_path);
	if (!(old_pwd_node->values)[0])
		return ;
}

/**
 *
 */
static void	ft_cd_home(t_env *env, char *old_path)
{
	char	*home_path;

	home_path = (get_env_node(env, "HOME")->values)[0];
	if (!home_path)
		return ;
	if (chdir(home_path) != 0)
		perror("cd");
	change_oldpwd(env, old_path);
}

/**
 *
 */
void	ft_cd(t_env *env, int argc, char **argv)
{
	char	old_path[PATH_MAX];
	char	path[PATH_MAX];

	if (getcwd(path, PATH_MAX) == NULL || getcwd(old_path, PATH_MAX) == NULL)
		return (perror("cd"));
	if (argc == 1)
		return (ft_cd_home(env, old_path));
	if (argc > 2)
		ft_printf("cd: too many arguments\n");
	if (argv[1][0] == '/')
	{
		if (chdir(argv[1]) != 0)
			perror("cd");
		change_oldpwd(env, old_path);
		return ;
	}
	if (ft_strlen(path) + ft_strlen(argv[1]) + 1 > PATH_MAX)
		return ((void) ft_printf("cd: ENAMETOOLONG\n"));
	ft_strlcat(path, "/", PATH_MAX);
	ft_strlcat(path, argv[1], PATH_MAX);
	if (chdir(path) != 0)
		perror("cd");
	change_oldpwd(env, old_path);
}
