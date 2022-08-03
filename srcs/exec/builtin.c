/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/05 15:52:12 by jcalon            #+#    #+#             */
/*   Updated: 2022/08/03 14:43:13 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_cd(t_separate *list)
{
	if (ft_array_size(list->cmds) > 2)
	{
		g_global.return_code = errmsg("cd: ", "too many args", NULL);
		return (1);
	}
	if (list->cmds[1][0] == '-' && list->cmds[1][1] == '\0')
	{
		if (chdir(ft_getenv("OLDPWD") + 7) == -1)
			g_global.return_code = errmsg("cd: ", "OLDPWD", ": no such file or directory");
		else
			g_global.return_code = 0;
	}
	else
	{
		if (chdir(list->cmds[1]) == -1)
			g_global.return_code = errmsg("cd: ", list->cmds[1], ": no such file or directory");
		else
			g_global.return_code = 0;
	}
	return (0);
}

int	builtin_pwd(t_separate *list)
{
	char	cwd[PATH_MAX];

	if (ft_array_size(list->cmds) != 1)
	{
		g_global.return_code = 2;
		errmsg("pwd: ", "too many args", NULL);
	}
	else if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		if (list->out)
		{
			ft_putendl_fd(cwd, list->fdout);
			close(list->fdout);
		}
		else
			ft_putendl_fd(cwd, 1);
		g_global.return_code = 0;
	}
	else
		g_global.return_code = errmsg("pwd: ", "unexpected error", NULL);
	return (0);
}

int	builtin_exit(t_separate *list)
{
	size_t	i;

	i = 0;
	if (list->cmds[1] && list->cmds[1][i] == '-')
		i++;
	if (list->cmds[1])
	{
		while (list->cmds[1][i])
		{
			if (!ft_isdigit(list->cmds[1][i]))
				break ;
			i++;
		}
		if (i != ft_strlen(list->cmds[1]) || ft_strlen(list->cmds[1]) > 12)
		{
			ft_putendl_fd("exit", 1);
			errmsg("exit: ", list->cmds[1], ": numeric argument required");
			exit(2);
		}
		else if (ft_array_size(list->cmds) == 2)
		{
			ft_putendl_fd("exit", 1);
			exit(ft_atoi(list->cmds[1]));
		}
		else
		{
			ft_putendl_fd("exit", 1);
			g_global.return_code = errmsg("exit: ", "too many args", NULL);
			exit(1);
		}
	}
	ft_putendl_fd("exit", 1);
	ft_free_array(g_global.env);
	exit(0);
	return (0);
}
