#include <stdio.h>
#include <limits.h>
#include <string.h>
#include "libft.h"
#include "get_next_line.h"
#include "minishell.h"


int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] != '\0')
	{
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	if (s1[i] != s2[i])
		return ((unsigned char)s1[i] - (unsigned char)s2[i]);
	return (0);
}

void ft_echo(int argc, char **argv)
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

void ft_pwd()
{
	char path[PATH_MAX];

	if (getcwd(path, PATH_MAX) == NULL)
		perror("pwd");
	else
		ft_printf("%s\n", path);
}

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


//returns a malloced string
//returns NULL on malloc error
char *get_key(const char *key_value)
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
const char *get_values_pos(const char *key_value)
{
	int		i;

	i = 0;
	while (key_value[i] && key_value[i] != '=')
		i++;
	if (key_value[i] == '\0')
		return (NULL);
	else
		return (key_value + i + 1);
}

static void	print_export(t_env *env)
{
	int	i;


	while(env)
	{
		ft_printf("declare -x %s=\"", env->key);
		i = 0;
		while ((env->values)[i])
		{
			if (i > 0)
				printf(":");
			printf("%s", (env->values)[i]);
			i++;
		}
		ft_printf("\"\n");
		env = env->next;
	}
}

void ft_export(t_env *env, int argc, char **argv)
{
	int			i;
	char		*key;
	const char	*values_pos;
	t_env		*env_node;

	if (argc == 1)
		return(print_export(env));
	i = 1;
	while (i < argc)
	{
		key = get_key(argv[i]);
		values_pos = get_values_pos(argv[i]);
		env_node = get_env_node(env, key);
		if (env_node)
		{
			free(key);
			free_str_arrays(env_node->values);
			env_node->values = ft_split(values_pos, ':');
		}
		else
			add_env_var(env, key, (char *)values_pos);
		i++;
	}
}

//int main(int argc, char **argv, char** envp)
//{
//	if (argc > 0)
//		ft_cd(argc, argv);
//	ft_pwd();
//}
