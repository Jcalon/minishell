/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/13 14:48:00 by jcalon            #+#    #+#             */
/*   Updated: 2022/07/13 14:48:19 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_env(bool export)
{
	size_t	i;

	i = 0;
	if (export == false)
	{
		while (g_global.env[i])
		{
			if (ft_strchr(g_global.env[i], '='))
				ft_putendl_fd(g_global.env[i], 1);
			i++;
		}	
	}
	else if (export == true)
	{
		while (g_global.env[i])
		{
			ft_putstr_fd("declare -x ", 1);
			ft_putendl_fd(g_global.env[i], 1);
			i++;
		}
	}
	return (0);
}
