#include <stdio.h>
#include <limits.h>
#include <string.h>
#include "../headers/libft.h"
#include "../headers/get_next_line.h"

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

void ft_echo(char **argv)
{
	int no_last_line;
	int i;

	// Check for -n option
	no_last_line = (argv[1] && ft_strcmp(argv[1], "-n") == 0);
	i = no_last_line + 1;
	// Print the arguments
	while (argv[i])
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
	char path[PATH_MAX];

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

int main(int argc, char **argv)
{
	if (argc > 0)
		ft_cd(argc, argv);
	ft_pwd();
}
