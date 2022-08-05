/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/07 14:53:46 by jcalon            #+#    #+#             */
/*   Updated: 2022/08/05 15:29:37 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Gestion des signaux dans un process,
il faut que SIGQUIT marche comme pour cat par ex */

void	handle_process(int sig_num)
{
	if (sig_num == SIGQUIT)
	{
		ft_putstr_fd(" Quit (core dumped)\n", 1);
		g_return_code = 131;
	}
	else if (sig_num == SIGINT)
	{
		ft_putstr_fd("\n", 1);
		g_return_code = 130;
	}
}

/* Gestion de SIGINT donc CTRL-C dans le processus parent */

void	handler(int sig_num)
{
	if (sig_num == SIGINT)
	{
		ft_putchar_fd('\n', 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		g_return_code = 130;
	}
}
