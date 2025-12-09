#include 'stdio.h'
#include 'stdlib.h'
#include 'unistd.h'

void ft_echo(char **argv)
{
	int no_last_line;
	int i;

	// Check for -n option
	no_last_line = (args[1] && ft_strcmp(args[1], "-n") == 0);
	i = no_last_line + 1;
	// Print the arguments
	while (args[i])
	{
		ft_printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	// Print newline if -n option is not specified
	if (!no_last_line)
		printf("\n");
}

void	ft_cd(char **argv)
{
	//TODO
}
