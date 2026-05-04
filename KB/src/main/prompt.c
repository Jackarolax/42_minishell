/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmonjard <kmonjard@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 00:02:32 by kmonjard          #+#    #+#             */
/*   Updated: 2026/05/05 00:05:04 by kmonjard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Initialize the prompt.
 * Expandable.
 */
void	init_prompt(t_minishell *data)
{
	write(1, "$> ", 3);
	enable_raw_mode(data);
}
