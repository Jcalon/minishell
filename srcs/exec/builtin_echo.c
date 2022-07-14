/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/14 13:51:03 by jcalon            #+#    #+#             */
/*   Updated: 2022/07/14 14:40:21 by jcalon           ###   ########.fr       */
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
				while (cmd[i][j] && cmd[i][j] == c)
				{
					c = 0;
					if ((cmd[i][j + 1] == '\'' || cmd[i][j + 1] == '\"'))
						c = cmd[i][j + 1];
					j++;
				}
			}
			// if (cmd[i][j] && cmd[i][j] == '$')
			// {
				
			// }
			if (cmd[i][j] && cmd[i][j] != c)
				write(1, &cmd[i][j], 1);
			if (cmd[i][j])
				j++;
		}
		if (cmd[i + 1] != NULL)
			write(1, " ", 1);
		c = 0;
		i++;
	}
	write(1, "\n", 1);
	g_global.return_code = 0;
	return (0);
}