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
 *changes PWD env variable

 */
static void	change_pwd(t_env *env)
{
	t_env		*old_pwd_node;
	const char	*export_argv[2];
	char		path[PATH_MAX];

	if (getcwd(path, PATH_MAX) == NULL)
		perror("pwd");
	export_argv[0] = "export";
	export_argv[1] = "PWD";
	old_pwd_node = get_env_node(env, "PWD");
	if (!old_pwd_node)
	{
		ft_export(env, 2, (char **) export_argv);
		old_pwd_node = get_env_node(env, "PWD");
	}
	if (old_pwd_node && old_pwd_node->values)
		free_str_arrays(old_pwd_node->values);
	if (old_pwd_node)
		old_pwd_node->values = ft_split(path, ':');
}

/**
 *changes OLDPWD env variable
 */
static void	change_oldpwd(t_env *env, char *old_path)
{
	t_env		*old_pwd_node;
	const char	*export_argv[2];

	export_argv[0] = "export";
	export_argv[1] = "OLDPWD";
	old_pwd_node = get_env_node(env, "OLDPWD");
	if (!old_pwd_node)
	{
		ft_export(env, 2, (char **) export_argv);
		old_pwd_node = get_env_node(env, "OLDPWD");
	}
	if (old_pwd_node && old_pwd_node->values)
		free_str_arrays(old_pwd_node->values);
	if (old_pwd_node)
		old_pwd_node->values = ft_split(old_path, ':');
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
	else if (argc > 2)
		ft_printf("cd: too many arguments\n");
	if (argv[1][0] == '/')
	{
		if (chdir(argv[1]) != 0)
			perror("cd");
		change_oldpwd(env, old_path);
		change_pwd(env);
		return ;
	}
	if (ft_strlen(path) + ft_strlen(argv[1]) + 1 > PATH_MAX)
		return ((void) ft_printf("cd: ENAMETOOLONG\n"));
	ft_strlcat(path, "/", PATH_MAX);
	ft_strlcat(path, argv[1], PATH_MAX);
	if (chdir(path) != 0)
		perror("cd");
	change_oldpwd(env, old_path);
	change_pwd(env);
}
