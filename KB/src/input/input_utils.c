#include "minishell.h"

// Removes the character just BEFORE the given position
char *delete_char(char *str, long pos)
{
	char	*new_str;
	long	len;
	long	i;
	long	j;

	if (!str || pos <= 0)
		return (str);
	len = ft_strlen(str);
	new_str = malloc(sizeof(char) * len);
	if (!new_str)
		return (NULL);
	i = 0;
	j = 0;
	while (str[i])
	{
		if (i == pos - 1) // Skip the character we are deleting
		{
			i++;
			continue;
		}
		new_str[j++] = str[i++];
	}
	new_str[j] = '\0';
	free(str);
	return (new_str);
}

// Inserts character 'c' into 'str' at the index 'pos'
char *insert_char(char *str, char c, long pos)
{
	char	*new_str;
	long	len;
	long	i;
	long	j;

	len = 0;
	if (str != NULL)
		len = ft_strlen(str);
	new_str = malloc(sizeof(char) * (len + 2));
	if (!new_str)
		return (NULL);
	i = 0;
	j = 0;
	while (i <= len)
	{
		if (i == pos)
			new_str[i] = c;
		else
			new_str[i] = str[j++];
		i++;
	}
	new_str[i] = '\0';
	if (str != NULL)
		free(str);
	return (new_str);
}
