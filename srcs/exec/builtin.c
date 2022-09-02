/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/05 15:52:12 by jcalon            #+#    #+#             */
/*   Updated: 2022/09/02 14:41:02 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	new_is_old(t_separate *list, t_data *pipex, size_t i, size_t j)
{
	char	cwd[PATH_MAX];

	free(list->begin->env[j]);
	list->begin->env[j] = ft_strjoin("OLD", list->begin->env[i]);
	if (!list->begin->env[j])
		ft_error(list, pipex, errmsg("Unexpected malloc error", "", ""));
	free(list->begin->env[i]);
	list->begin->env[i] = ft_strjoin("PWD=", getcwd(cwd, sizeof(cwd)));
	if (!list->begin->env[i])
		ft_error(list, pipex, errmsg("Unexpected malloc error", "", ""));
}

static void	update_env(t_separate *list, t_data *pipex)
{
	size_t	i;
	size_t	j;

	i = 0;
	g_status = 0;
	while (list->begin->env[i])
	{
		if (!ft_strncmp(list->begin->env[i], "PWD=", 4))
			break ;
		i++;
	}
	j = 0;
	while (list->begin->env[j])
	{
		if (!ft_strncmp(list->begin->env[j], "OLDPWD=", 6))
			break ;
		j++;
	}
	if (list->begin->env[i] && list->begin->env[j])
		new_is_old(list, pipex, i, j);
}

void	builtin_cd(t_separate *list, t_data *pipex)
{
	char	**cmds;

	if (pipex)
		cmds = pipex->cmd;
	else
		cmds = list->cmds;
	if (ft_array_size(cmds) > 2)
		g_status = errmsg("cd: ", "too many args", NULL);
	else if (ft_array_size(cmds) > 1)
	{
		if (chdir(cmds[1]) == -1)
		{
			g_status = errmsg("cd: ", cmds[1], ": No such file or directory");
			return ;
		}
	}
	else
	{
		if (chdir("/") == -1)
		{
			g_status = errmsg("cd: ", cmds[1], ": No such file or directory");
			return ;
		}
	}
	update_env(list, pipex);
}

void	builtin_pwd(t_separate *list)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		ft_putendl_fd(cwd, test_fdout(list));
		g_status = 0;
	}
	else
		g_status = errmsg("pwd: ", "unexpected error", NULL);
}
