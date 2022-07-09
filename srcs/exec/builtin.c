/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/05 15:52:12 by jcalon            #+#    #+#             */
/*   Updated: 2022/07/09 17:05:10 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	builtin_echo(char *path)
// {
// 	if (path == NULL)
// 		ft_printf("\n");
// 	else
// 		ft_printf("%s\n", path);
// }

void	builtin_echo(char **cmd)
{
	size_t	i;
	size_t	j;
	char	c;

	i = 1;
	c = 0;
	while (cmd[i])
	{
		j = 0;
		while (cmd[i][j])
		{
			if (cmd[i][j] == c)
			{
				c = 0;
				j++;
			}
			if (c == 0 && (cmd[i][j] == '\'' || cmd[i][j] == '\"'))
			{
				c = cmd[i][j];
				j++;
			}
			if (cmd[i][j] != c)
				write(1, &cmd[i][j], 1);
			if (cmd[i][j])
				j++;
		}
		if (cmd[i + 1] != NULL)
			write(1, " ", 1);
		i++;
	}
	write(1, "\n", 1);
}

void	builtin_cd(char	*path)
{
	if (chdir(path) == -1)
		perror("cd");
}

void	builtin_pwd(void)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
		ft_printf("%s\n", cwd);
	else
		perror("pwd");
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
