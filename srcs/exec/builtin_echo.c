/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/14 13:51:03 by jcalon            #+#    #+#             */
/*   Updated: 2022/08/05 15:10:12 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	check_n(char **cmd)
{
	size_t	i;
	size_t	j;

	i = 1;
	while (cmd[i] && cmd[i][0] == '-' && cmd[i][1] == 'n')
	{
		j = 1;
		while (cmd[i][j])
		{
			if (cmd[i][j] != 'n')
				return (i);
			j++;
		}
		i++;
	}
	return (i);
}

int	builtin_echo(t_separate *list, t_data *pipex)
{
	size_t	i;
	size_t	j;
	size_t	k;
	char	c;
	bool	n;
	char	**cmds;

	if (pipex)
		cmds = pipex->cmd;
	else
		cmds = list->cmds;
	c = 0;
	n = false;
	i = check_n(cmds);
	if (i > 1)
		n = true;
	while (cmds[i])
	{
		j = 0;
		while (cmds[i][j])
		{
			if (cmds[i][j] == c)
			{
				c = 0;
				j++;
			}
			if (c == 0 && (cmds[i][j] == '\'' || cmds[i][j] == '\"'))
			{
				c = cmds[i][j];
				j++;
				while (cmds[i][j] && cmds[i][j] == c)
				{
					c = 0;
					if ((cmds[i][j + 1] == '\'' || cmds[i][j + 1] == '\"'))
						c = cmds[i][j + 1];
					j++;
				}
			}
			if (cmds[i][j] && cmds[i][j] == '$' && c != '\'')
			{
				if (cmds[i][j + 1] && cmds[i][j + 1] == '?')
				{
					if (list->out)
						ft_putnbr_fd(g_return_code, list->fdout);
					else
						ft_putnbr_fd(g_return_code, 1);
					j += 2;
				}
				else if (cmds[i][j + 1] && ft_isalpha(cmds[i][j + 1]))
				{
					k = 0;
					j++;
					while (ft_isalnum(cmds[i][j + k]))
						k++;
					if (list->out)
						ft_putstr_fd(ft_get_var_env(list, cmds[i] + j, k), list->fdout);
					else
						ft_putstr_fd(ft_get_var_env(list, cmds[i] + j, k), 1);
					j += k - 1;
				}
				else
				{
					if (list->out && !ft_isdigit(cmds[i][j + 1]) && cmds[i][j + 1] != '\"' && cmds[i][j + 1] != '\'')
						write(list->fdout, "$", 1);
					else if (!ft_isdigit(cmds[i][j + 1]) && cmds[i][j + 1] != '\"' && cmds[i][j + 1] != '\'')
						write(1, "$", 1);
					j++;
					while (cmds[i][j] && (cmds[i][j] == '$' || ft_isdigit(cmds[i][j])))
						j++;
					j--;
				}
			}
			else if (cmds[i][j] && cmds[i][j] != c)
			{
				if (list->out)
					write(list->fdout, &cmds[i][j], 1);
				else
					write(1, &cmds[i][j], 1);
			}
			if (cmds[i][j])
				j++;
		}
		if (cmds[i + 1] != NULL)
		{
			if (list->out)
				write(list->fdout, " ", 1);
			else
				write(1, " ", 1);
		}
		c = 0;
		i++;
	}
	if (n == false)
	{
		if (list->out)
			write(list->fdout, "\n", 1);
		else
			write(1, "\n", 1);
	}
	if (list->out)
		close(list->fdout);
	g_return_code = 0;
	return (0);
}
