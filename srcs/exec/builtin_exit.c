/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/02 11:34:47 by jcalon            #+#    #+#             */
/*   Updated: 2022/09/02 14:40:56 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	exit_too_many_arg(void)
{
	ft_putendl_fd("exit", 1);
	g_status = errmsg("exit: ", "too many args", NULL);
	return (1);
}

static int	exit_with_args(t_separate *list, char **cmds, size_t i)
{
	int		exit_value;

	if (i != ft_strlen(cmds[1]) || ft_strlen(cmds[1]) > 12)
	{
		ft_putendl_fd("exit", 1);
		errmsg("exit: ", cmds[1], ": numeric argument required");
		ft_free_array(list->begin->env);
		free_stuff(list);
		close_std();
		exit(2);
	}
	else if (ft_array_size(cmds) == 2)
	{
		ft_putendl_fd("exit", 1);
		exit_value = ft_atoi(cmds[1]);
		ft_free_array(list->begin->env);
		free_stuff(list);
		close_std();
		exit(exit_value);
	}
	else
		return (exit_too_many_arg());
	return (0);
}

void	builtin_exit(t_separate *list, t_data *pipex)
{
	size_t	i;
	char	**cmds;

	if (pipex)
		cmds = pipex->cmd;
	else
		cmds = list->cmds;
	i = 0;
	if (cmds[1])
	{
		if (cmds[1][i] == '-')
			i++;
		while (cmds[1][i])
		{
			if (!ft_isd(cmds[1][i]))
				break ;
			i++;
		}
		if (exit_with_args(list, cmds, i))
			return ;
	}
	ft_putendl_fd("exit", 1);
	g_status = 0;
	ft_quit(list);
}
