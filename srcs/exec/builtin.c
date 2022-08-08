/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/05 15:52:12 by jcalon            #+#    #+#             */
/*   Updated: 2022/08/08 15:00:41 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	builtin_cd(t_separate *list, t_data *pipex)
{
	char	**cmds;

	if (pipex)
		cmds = pipex->cmd;
	else
		cmds = list->cmds;
	if (ft_array_size(cmds) > 2)
		g_return_code = errmsg("cd: ", "too many args", NULL);
	if (cmds[1][0] == '-' && cmds[1][1] == '\0')
	{
		if (chdir(ft_getenv(list, "OLDPWD") + 7) == -1)
			g_return_code = errmsg("cd: ", \
				"OLDPWD", ": no such file or directory");
		else
			g_return_code = 0;
	}
	else
	{
		if (chdir(cmds[1]) == -1)
			g_return_code = errmsg("cd: ", \
				cmds[1], ": no such file or directory");
		else
			g_return_code = 0;
	}
}

void	builtin_pwd(t_separate *list, t_data *pipex)
{
	char	cwd[PATH_MAX];
	char	**cmds;

	if (pipex)
		cmds = pipex->cmd;
	else
		cmds = list->cmds;
	if (ft_array_size(cmds) != 1)
	{
		g_return_code = 2;
		errmsg("pwd: ", "too many args", NULL);
	}
	else if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		ft_putendl_fd(cwd, test_fdout(list));
		g_return_code = 0;
	}
	else
		g_return_code = errmsg("pwd: ", "unexpected error", NULL);
}

void	close_std(void)
{
	close(STDERR_FILENO);
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
}

static void	exit_with_args(t_separate *list, char **cmds, size_t i)
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
	{
		ft_putendl_fd("exit", 1);
		g_return_code = errmsg("exit: ", "too many args", NULL);
	}
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
		exit_with_args(list, cmds, i);
	}
	ft_putendl_fd("exit", 1);
	ft_free_array(list->begin->env);
	free_stuff(list);
	close_std();
	exit(0);
}
