/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crazyd <crazyd@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/14 13:51:03 by jcalon            #+#    #+#             */
/*   Updated: 2022/07/21 09:43:08 by crazyd           ###   ########.fr       */
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

int	builtin_echo(t_separate *list)
{
	size_t	i;
	size_t	j;
	size_t	k;
	char	c;
	bool	n;

	c = 0;
	n = false;
	i = check_n(list->cmds);
	if (i > 1)
		n = true;
	while (list->cmds[i])
	{
		j = 0;
		while (list->cmds[i][j])
		{
			if (list->cmds[i][j] == c)
			{
				c = 0;
				j++;
			}
			if (c == 0 && (list->cmds[i][j] == '\'' || list->cmds[i][j] == '\"'))
			{
				c = list->cmds[i][j];
				j++;
				while (list->cmds[i][j] && list->cmds[i][j] == c)
				{
					c = 0;
					if ((list->cmds[i][j + 1] == '\'' || list->cmds[i][j + 1] == '\"'))
						c = list->cmds[i][j + 1];
					j++;
				}
			}
			if (list->cmds[i][j] && list->cmds[i][j] == '$' && c != '\'')
			{
				if (list->cmds[i][j + 1] && list->cmds[i][j + 1] == '?')
				{
					if (list->out)
						ft_putnbr_fd(g_global.return_code, list->fdout);
					else
						ft_putnbr_fd(g_global.return_code, 1);
					j += 2;
				}
				else if (list->cmds[i][j + 1] && ft_isalpha(list->cmds[i][j + 1]))
				{
					k = 0;
					j++;
					while (ft_isalnum(list->cmds[i][j + k]))
						k++;
					if (list->out)
						ft_putstr_fd(ft_get_var_env(list->cmds[i] + j, k), list->fdout);
					else
						ft_putstr_fd(ft_get_var_env(list->cmds[i] + j, k), 1);
					j += k - 1;
				}
				else if (list->cmds[i][j + 1] != '\'' && list->cmds[i][j + 1] != '\"')
				{
					if (list->out)
						write(list->fdout, "$", 1);
					else
						write(1, "$", 1);
					j++;
					while (list->cmds[i][j] && (list->cmds[i][j] == '$' || ft_isdigit(list->cmds[i][j])))
						j++;
					j--;
				}
			}
			else if (list->cmds[i][j] && list->cmds[i][j] != c)
			{
				if (list->out)
					write(list->fdout, &list->cmds[i][j], 1);
				else
					write(1, &list->cmds[i][j], 1);
			}
			if (list->cmds[i][j])
				j++;
		}
		if (list->cmds[i + 1] != NULL)
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
	g_global.return_code = 0;
	return (0);
}