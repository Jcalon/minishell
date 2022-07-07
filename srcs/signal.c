/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/07 14:53:46 by jcalon            #+#    #+#             */
/*   Updated: 2022/07/07 17:01:53 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_process(int sig_num)
{
	if (!kill(g_global.child_pid, sig_num))
	{
		if (sig_num == SIGQUIT)
		{
			ft_putstr_fd("Quit: 3\n", 1);
			g_global.return_code = 131;
		}
		else if (sig_num == SIGINT)
		{
			ft_putchar_fd('\n', 1);
			g_global.return_code = 130;
		}
	}
	else if (sig_num == SIGINT)
	{
		ft_putchar_fd('\n', 1);
		g_global.return_code = 1;
	}	
}

void	handler(int sig_num)
{
	if (g_global.child_pid)
		handle_process(sig_num);
	else if (sig_num == SIGINT)
	{
		ft_putchar_fd('\n', 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else if (sig_num == SIGQUIT)
		ft_putstr_fd("\b\b  \b\b", 1);
}
