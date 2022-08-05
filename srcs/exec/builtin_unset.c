/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/13 14:47:48 by jcalon            #+#    #+#             */
/*   Updated: 2022/08/05 11:17:17 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	ft_strlen_equal(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i] != '\0')
	{
		if (s[i] == '=')
			break ;
		i++;
	}
	if (s[i] == '=' && s[i - 1] == '+')
		return (i - 1);
	return (i);
}

int	check_double_env(char *str, size_t len)
{
	size_t	i;

	i = 0;
	while (g_global.env[i])
	{
		if (ft_strnstr(g_global.env[i], str, len)
			&& (ft_strlen_equal(g_global.env[i]) == ft_strlen_equal(str)))
			return (1);
		i++;
	}
	return (0);
}

size_t	num_to_unset(char	**cmd)
{
	size_t	i;
	size_t	count;

	i = 1;
	count = 0;
	while (cmd[i])
	{
		if (!ft_isalpha(cmd[i][0]))
		{
			g_global.return_code = errmsg("unset: ", cmd[i], ": not a valid identifier");
			return (0);
		}
		if (ft_isalpha(cmd[i][0])
				&& check_double_env(cmd[i], ft_strlen_equal(cmd[i])))
		{
			count++;	
		}
		i++;
	}
	return (count);
}

int	builtin_unset(t_separate *list, t_data *pipex)
{
	size_t	i;
	size_t	j;
	size_t	k;
	size_t	size;
	char	**new_env;
	bool	dup;
	char	**cmds;

	if (pipex)
		cmds = pipex->cmd;
	else
		cmds = list->cmds;
	if (g_global.env[0] == NULL)
	{
		j = 1;
		while (cmds[j])
		{
			if (!ft_isalpha(cmds[j][0]))
				g_global.return_code = errmsg("unset: ", cmds[j], ": not a valid identifier");	
			j++;
		}
		return (0);
	}
	size = ft_array_size(g_global.env) - num_to_unset(cmds) + 1;
	new_env = malloc(sizeof(char *) * size);
	i = 0;
	k = 0;
	while (g_global.env[i])
	{
		j = 1;
		dup = true;
		while (cmds[j])
		{	
			if (ft_strnstr(g_global.env[i], cmds[j], ft_strlen(cmds[j]))
				&& (ft_strlen_equal(g_global.env[i]) == ft_strlen(cmds[j])))
				dup = false;
			j++;
		}
		if (dup == true)
			new_env[k++] = ft_strdup(g_global.env[i]);
		i++;
	}
	new_env[k] = NULL;
	ft_free_array(g_global.env);
	g_global.env = new_env;
	return (0);
}
