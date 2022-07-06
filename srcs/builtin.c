/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/05 15:52:12 by jcalon            #+#    #+#             */
/*   Updated: 2022/07/05 16:36:21 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	builtin_echo(char *path)
{
	ft_printf("%s\n", path);
}

void	builtin_cd(char	*path)
{
	if (chdir(path) == -1)
		perror("chdir()");
}

void	builtin_pwd(void)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
		ft_printf("%s\n", cwd);
	else
		perror("getcwd()");
}

void	builtin_export(void)
{
	ft_printf("export\n");
}

void	builtin_unset(void)
{
	ft_printf("unset\n");
}

void	builtin_env(void)
{
	ft_printf("env\n");
}

void	builtin_exit(void)
{
	ft_printf("exit\n");
}
