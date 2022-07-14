/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/13 11:31:30 by jcalon            #+#    #+#             */
/*   Updated: 2022/07/14 12:40:38 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	num_to_export(char	**cmd)
{
	size_t	i;
	size_t	count;

	i = 1;
	count = 0;
	while (cmd[i])
	{
		if (ft_isalpha(cmd[i][0]) && !check_double_env(cmd[i], ft_strlen_equal(cmd[i])))
			if (cmd[i][ft_strlen_equal(cmd[i])] != '+')
				count++;
		i++;
	}
	return (count);
}

void	update_double(char	**cmd)
{
	size_t	i;
	size_t	j;

	j = 0;
	while (g_global.env[j])
	{
		i = 1;
		while (cmd[i])
		{
			if (!ft_strncmp(g_global.env[j], cmd[i], ft_strlen_equal(cmd[i]))
				&& ft_strlen_equal(g_global.env[j]) == ft_strlen_equal(cmd[i])
					&& cmd[i][ft_strlen_equal(cmd[i]) + 1] == '='
						&& cmd[i][ft_strlen_equal(cmd[i]) + 2] != '\0'
							&& cmd[i][ft_strlen_equal(cmd[i])] == '+')
			{
				ft_join_more(&g_global.env[j], (cmd[i] + ft_strlen_equal(cmd[i]) + 2));
			}
			else if (!ft_strncmp(g_global.env[j], cmd[i], ft_strlen_equal(cmd[i]))
				&& ft_strlen_equal(g_global.env[j]) == ft_strlen_equal(cmd[i])
					&& cmd[i][ft_strlen_equal(cmd[i])] == '='
						&& cmd[i][ft_strlen_equal(cmd[i]) + 1] != '\0')
			{
				free(g_global.env[j]);
				g_global.env[j] = ft_strdup(cmd[i]);
			}
			i++;
		}
		j++;
	}
}

int	builtin_export(char	**cmd)
{
	size_t	i;
	size_t	j;
	size_t	size;
	int		err;
	char	**new_env;

	err = 0;
	if (!cmd[1])
		builtin_env(true);
	else
	{
		i = 0;
		if (g_global.env[0] == NULL)
			size = num_to_export(cmd) + 1;
		else
			size = num_to_export(cmd) + ft_array_size(g_global.env) + 1;
		new_env = malloc(sizeof(char *) * size);
		while (g_global.env[i])
		{
			new_env[i] = ft_strdup(g_global.env[i]);
			i++;
		}
		j = 1;
		while (cmd[j])
		{
			if (!ft_isalpha(cmd[j][0]))
				err = errmsg("export: ", cmd[j], ": not a valid identifier");	
			else if (!check_double_env(cmd[j], ft_strlen_equal(cmd[j])))
				new_env[i++] = ft_strdup(cmd[j]);
			j++;
		}
		new_env[i] = NULL;
		ft_free_array(g_global.env);
		g_global.env = new_env;
	}
	update_double(cmd);
	g_global.return_code = err;
	return (0);
}