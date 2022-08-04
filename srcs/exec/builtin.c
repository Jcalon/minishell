/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/05 15:52:12 by jcalon            #+#    #+#             */
/*   Updated: 2022/08/04 21:31:21 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_cd(t_separate *list, t_data *pipex)
{
	char	**cmds;

	if (pipex)
		cmds = pipex->cmd;
	else
		cmds = list->cmds;
	if (ft_array_size(cmds) > 2)
	{
		g_global.return_code = errmsg("cd: ", "too many args", NULL);
		return (1);
	}
	if (cmds[1][0] == '-' && cmds[1][1] == '\0')
	{
		if (chdir(ft_getenv("OLDPWD") + 7) == -1)
			g_global.return_code = errmsg("cd: ", "OLDPWD", ": no such file or directory");
		else
			g_global.return_code = 0;
	}
	else
	{
		if (chdir(cmds[1]) == -1)
			g_global.return_code = errmsg("cd: ", cmds[1], ": no such file or directory");
		else
			g_global.return_code = 0;
	}
	return (0);
}

int	builtin_pwd(t_separate *list, t_data *pipex)
{
	char	cwd[PATH_MAX];
	char	**cmds;

	if (pipex)
		cmds = pipex->cmd;
	else
		cmds = list->cmds;
	if (ft_array_size(cmds) != 1)
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

int	builtin_exit(t_separate *list, t_data *pipex)
{
	size_t	i;
	int		exit_value;
	char	**cmds;

	if (pipex)
		cmds = pipex->cmd;
	else
		cmds = list->cmds;
	i = 0;
	if (cmds[1] && cmds[1][i] == '-')
		i++;
	if (cmds[1])
	{
		while (cmds[1][i])
		{
			if (!ft_isdigit(cmds[1][i]))
				break ;
			i++;
		}
		if (i != ft_strlen(cmds[1]) || ft_strlen(cmds[1]) > 12)
		{
			ft_putendl_fd("exit", 1);
			errmsg("exit: ", cmds[1], ": numeric argument required");
			free_stuff(list);
			ft_free_array(g_global.env);
			exit(2);
		}
		else if (ft_array_size(cmds) == 2)
		{
			ft_putendl_fd("exit", 1);
			exit_value = ft_atoi(cmds[1]);
			free_stuff(list);
			ft_free_array(g_global.env);
			exit(exit_value);
		}
		else
		{
			ft_putendl_fd("exit", 1);
			g_global.return_code = errmsg("exit: ", "too many args", NULL);
			free_stuff(list);
			ft_free_array(g_global.env);
			exit(1);
		}
	}
	ft_putendl_fd("exit", 1);
	free_stuff(list);
	ft_free_array(g_global.env);
	exit(0);
	return (0);
}
