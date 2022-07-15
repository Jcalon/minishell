/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/05 15:52:12 by jcalon            #+#    #+#             */
/*   Updated: 2022/07/15 12:18:54 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_cd(char	**path)
{
	if (ft_array_size(path) > 2)
	{
		g_global.return_code = errmsg("cd: ", "too many args", NULL);
		return (1);
	}
	if (path[1][0] == '-' && path[1][1] == '\0')
	{
		if (chdir(ft_getenv("OLDPWD") + 7) == -1)
			g_global.return_code = errmsg("cd: ", "OLDPWD", ": no such file or directory");
		else
			g_global.return_code = 0;
	}
	else
	{
		if (chdir(path[1]) == -1)
			g_global.return_code = errmsg("cd: ", path[1], ": no such file or directory");
		else
			g_global.return_code = 0;
	}
	return (0);
}

int	builtin_pwd(char **cmd)
{
	char	cwd[PATH_MAX];

	if (ft_array_size(cmd) != 1)
	{
		g_global.return_code = 2;
		errmsg("pwd: ", "too many args", NULL);
	}
	else if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		ft_printf("%s\n", cwd);
		g_global.return_code = 0;
	}
	else
		g_global.return_code = errmsg("pwd: ", "unexpected error", NULL);
	return (0);
}

int	builtin_exit(char **status)
{
	size_t	i;

	i = 0;
	if (status[1][i] == '-')
		i++;
	while (status[1][i])
	{
		if (!ft_isdigit(status[1][i]))
			break ;
		i++;
	}
	if (i != ft_strlen(status[1]) || ft_strlen(status[1]) > 12)
	{
		ft_putendl_fd("exit", 1);
		errmsg("exit: ", status[1], ": numeric argument required");
		exit(2);
	}
	else if (ft_array_size(status) == 2)
	{
		ft_putendl_fd("exit", 1);
		exit(ft_atoi(status[1]));
	}
	else
	{
		ft_putendl_fd("exit", 1);
		g_global.return_code = errmsg("exit: ", "too many args", NULL);
		return (1);
	}
	return (0);
}
