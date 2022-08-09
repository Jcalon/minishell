/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/13 14:48:00 by jcalon            #+#    #+#             */
/*   Updated: 2022/08/09 15:48:38 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Afficher l'environnement
	Si export, afficher même les variables sans '=' */

static void	builtin_env_export(t_separate *list)
{
	size_t	i;

	i = 0;
	while (list->begin->env[i])
	{
		if (list->out)
		{
			ft_putstr_fd("declare -x ", list->fdout);
			ft_putendl_fd(list->begin->env[i], list->fdout);
			close(list->fdout);
		}
		else
		{
			ft_putstr_fd("declare -x ", 1);
			ft_putendl_fd(list->begin->env[i], 1);
		}
		i++;
	}
}

void	builtin_env(bool export, t_separate *list)
{
	size_t	i;

	i = 0;
	if (export == false)
	{
		while (list->begin->env[i])
		{
			if (ft_strchr(list->begin->env[i], '='))
				ft_putendl_fd(list->begin->env[i], test_fdout(list));
			i++;
		}	
	}
	else if (export == true)
		builtin_env_export(list);
}
