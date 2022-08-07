/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/13 14:47:48 by jcalon            #+#    #+#             */
/*   Updated: 2022/08/07 18:02:24 by jcalon           ###   ########.fr       */
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

int	check_double_env(t_separate *list, char *str, size_t len)
{
	size_t	i;

	i = 0;
	while (list->begin->env[i])
	{
		if (ft_strnstr(list->begin->env[i], str, len)
			&& (ft_strlen_equal(list->begin->env[i]) == ft_strlen_equal(str)))
			return (1);
		i++;
	}
	return (0);
}

size_t	num_to_unset(t_separate *list, char	**cmd)
{
	size_t	i;
	size_t	count;

	i = 1;
	count = 0;
	while (cmd[i])
	{
		if (!ft_isalpha(cmd[i][0]))
		{
			g_return_code = errmsg("unset: ", \
				cmd[i], ": not a valid identifier");
			return (0);
		}
		if (ft_isalpha(cmd[i][0])
				&& check_double_env(list, cmd[i], ft_strlen_equal(cmd[i])))
		{
			count++;
		}
		i++;
	}
	return (count);
}

static void	ft_unset(t_separate *list, char **cmds, char **new_env)
{
	size_t	index[3];
	bool	dup;

	index[0] = 0;
	index[2] = 0;
	while (list->begin->env[index[0]])
	{
		index[1] = 1;
		dup = true;
		while (cmds[index[1]])
		{	
			if (ft_strnstr(list->begin->env[index[index[1]]], \
				cmds[index[1]], ft_strlen(cmds[index[1]]))
				&& (ft_strlen_equal(list->begin->env[index[0]]) \
				== ft_strlen(cmds[index[1]])))
				dup = false;
			index[1]++;
		}
		if (dup == true)
			new_env[index[2]++] = ft_strdup(list->begin->env[index[0]]);
		index[0]++;
	}
	new_env[--index[2]] = NULL;
	ft_free_array(list->begin->env);
	list->begin->env = new_env;
}

void	builtin_unset(t_separate *list, t_data *pipex)
{
	char	**cmds;
	char	**new_env;
	size_t	size;
	size_t	i;

	if (pipex)
		cmds = pipex->cmd;
	else
		cmds = list->cmds;
	if (list->begin->env[0] == NULL)
	{
		i = 1;
		while (cmds[i])
		{
			if (!ft_isalpha(cmds[i][0]))
				g_return_code = errmsg("unset: ", \
				cmds[i], ": not a valid identifier");
			i++;
		}
		return ;
	}
	size = ft_array_size(list->begin->env) - num_to_unset(list, cmds) + 1;
	new_env = malloc(sizeof(char *) * size);
	ft_unset(list, cmds, new_env);
}
