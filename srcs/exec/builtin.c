/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/05 15:52:12 by jcalon            #+#    #+#             */
/*   Updated: 2022/07/13 11:41:11 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_echo(char **cmd)
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
	return (0);
}

int	builtin_cd(char	*path)
{
	if (chdir(path) == -1)
		perror("cd");
	return (0);
}

int	builtin_pwd(void)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
		ft_printf("%s\n", cwd);
	else
		perror("pwd");
	return (0);
}

int	builtin_exit(char *status)
{
	size_t	i;

	i = 0;
	while (status[i])
	{
		if (!ft_isdigit(status[i]))
			break ;
		i++;
	}
	if (i != ft_strlen(status))
	{
		printf("minishell: exit: %s: numeric argument required\n", status);
		ft_putendl_fd("exit", 2);
		exit(2);
	}
	else
	{
		ft_putendl_fd("exit", 2);
		exit(ft_atoi(status));
	}
	return (0);
}
