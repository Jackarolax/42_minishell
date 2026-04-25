#include "minishell.h"

/**
 * @brief Wrap the disable for outer terminal.
 */
void	disable_raw_mode(t_minishell *data)
{
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &data->orig_settings);
}

/**
 * @brief Enable raw attributes for current terminal.
 */
void	enable_raw_mode(t_minishell *data)
{
	struct termios raw;
	tcgetattr(STDIN_FILENO, &data->orig_settings);
	raw = data->orig_settings;
	raw.c_lflag &= ~(ECHO | ICANON);
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}
