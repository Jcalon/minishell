/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/14 13:51:03 by jcalon            #+#    #+#             */
/*   Updated: 2022/07/15 11:56:49 by jcalon           ###   ########.fr       */
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

int	builtin_echo(char **cmd)
{
	size_t	i;
	size_t	j;
	size_t	k;
	char	c;
	bool	n;

	c = 0;
	n = false;
	i = check_n(cmd);
	if (i > 1)
		n = true;
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
				while (cmd[i][j] && cmd[i][j] == c)
				{
					c = 0;
					if ((cmd[i][j + 1] == '\'' || cmd[i][j + 1] == '\"'))
						c = cmd[i][j + 1];
					j++;
				}
			}
			if (cmd[i][j] && cmd[i][j] == '$' && c != '\'')
			{
				if (cmd[i][j + 1] && cmd[i][j + 1] == '?')
				{
					ft_putnbr_fd(g_global.return_code, 1);
					j += 2;
				}
				else if (cmd[i][j + 1] && ft_isalpha(cmd[i][j + 1]))
				{
					k = 0;
					j++;
					while (ft_isalnum(cmd[i][j + k]))
						k++;
					ft_putstr_fd(ft_get_var_env(cmd[i] + j, k), 1);
					j += k - 1;
				}
				else if (cmd[i][j + 1] != '\'' && cmd[i][j + 1] != '\"')
				{
					write(1, "$", 1);
					j++;
					while (cmd[i][j] && (cmd[i][j] == '$' || ft_isdigit(cmd[i][j])))
						j++;
					j--;
				}
			}
			else if (cmd[i][j] && cmd[i][j] != c)
				write(1, &cmd[i][j], 1);
			if (cmd[i][j])
				j++;
		}
		if (cmd[i + 1] != NULL)
			write(1, " ", 1);
		c = 0;
		i++;
	}
	if (n == false)
		write(1, "\n", 1);
	g_global.return_code = 0;
	return (0);
}