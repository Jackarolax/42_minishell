#include <stdio.h>
#include <limits.h>
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
		ft_printf("pwd error\n");
	else
		ft_printf("%s\n", path);
}

void	ft_cd(char **argv)
{
	char path[PATH_MAX];

	if (getcwd(path, PATH_MAX) == NULL)
		ft_printf("pwd error\n");

}

int main(int argc, char **argv)
{
	if (argc > 1)
		ft_echo(argv);
}
