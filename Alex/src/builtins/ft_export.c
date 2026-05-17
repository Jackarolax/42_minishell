/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anematol <anematol@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/17 14:07:35 by anematol          #+#    #+#             */
/*   Updated: 2026/05/17 17:25:31 by anematol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <limits.h>
#include <string.h>
#include "libft.h"
#include "get_next_line.h"
#include "minishell.h"

//returns a malloced string
//returns NULL on malloc error
char	*get_key(const char *key_value)
{
	int		i;
	char	*key;

	i = 0;
	while (key_value[i] && key_value[i] != '=')
		i++;
	key = malloc(sizeof(char) * (i + 1));
	if (!key)
		return (NULL);
	ft_strncpy(key, key_value, i);
	return (key);
}

//returns a pointer to the start of values
//so basically a pointer to the firts character after "="
//if there is no "=", NULL is returned
static const char	*get_values_pos(const char *key_value)
{
	int	i;

	i = 0;
	while (key_value[i] && key_value[i] != '=')
		i++;
	if (key_value[i] == '\0')
		return (key_value + i);
	else
		return (key_value + i + 1);
}

static void	print_export(t_env *env)
{
	int	i;

	while (env)
	{
		ft_printf("declare -x %s=\"", env->key);
		i = 0;
		while ((env->values)[i])
		{
			if (i > 0)
				ft_printf(":");
			ft_printf("%s", (env->values)[i]);
			i++;
		}
		ft_printf("\"\n");
		env = env->next;
	}
}

static void	export_error(int errornum, char *key_value)
{
	if (errornum == 1)
		ft_printf("export: `%s': not a valid identifier\n", key_value);
}

void	ft_export(t_env *env, int argc, char **argv)
{
	int		i;
	char	*key;
	t_env	*env_node;

	if (argc == 1)
		return (print_export(env));
	i = 1;
	while (i < argc)
	{
		key = get_key(argv[i]);
		if (!key)
			return ;
		if (key[0] == '\0' || !ft_isalpha(key[0]))
			return (free(key), export_error(1, argv[i]));
		env_node = get_env_node(env, key);
		if (env_node)
		{
			free_str_arrays(env_node->values);
			env_node->values = ft_split(get_values_pos(argv[i]), ':');
		}
		else
			add_env_var(env, key, (char *)get_values_pos(argv[i]));
		free(key);
		i++;
	}
}
