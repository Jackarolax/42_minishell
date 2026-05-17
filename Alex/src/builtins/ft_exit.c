/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anematol <anematol@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/17 14:42:13 by anematol          #+#    #+#             */
/*   Updated: 2026/05/17 15:00:13 by anematol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"


void	ft_exit(t_minishell *data_p)
{
	cleanup_loop(data_p);
	cleanup_shell(data_p);
	exit(g_signal);
}
