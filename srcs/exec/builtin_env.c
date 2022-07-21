/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crazyd <crazyd@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/13 14:48:00 by jcalon            #+#    #+#             */
/*   Updated: 2022/07/21 09:35:06 by crazyd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_env(bool export, t_separate *list)
{
	size_t	i;

	i = 0;
	if (export == false)
	{
		while (g_global.env[i])
		{
			if (ft_strchr(g_global.env[i], '='))
			{
				if (list->out)
				{
					ft_putendl_fd(g_global.env[i], list->fdout);
					close(list->fdout);
				}
				else
					ft_putendl_fd(g_global.env[i], 1);
			}
			i++;
		}	
	}
	else if (export == true)
	{
		while (g_global.env[i])
		{
			if (list->out)
			{
				ft_putstr_fd("declare -x ", list->fdout);
				ft_putendl_fd(g_global.env[i], list->fdout);
				close(list->fdout);
			}
			else
			{
				ft_putstr_fd("declare -x ", 1);
				ft_putendl_fd(g_global.env[i], 1);
			}
			i++;
		}
	}
	return (0);
}
