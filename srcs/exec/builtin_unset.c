/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/13 14:47:48 by jcalon            #+#    #+#             */
/*   Updated: 2022/09/02 14:41:00 by jcalon           ###   ########.fr       */
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
		if (ft_strchrstr(list->begin->env[i], str, len)
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
			g_status = errmsg("unset: ", \
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

void	ft_unset(t_separate *list, t_data *pipex, char **cmds, size_t size)
{
	int		i;
	size_t	j;
	size_t	k;
	char	**new_env;
	bool	dup;

	new_env = malloc(sizeof(char *) * size);
	if (!new_env)
		ft_error(list, pipex, errmsg("Unexpected malloc error", "", ""));
	i = -1;
	k = 0;
	while (list->begin->env[++i])
	{
		j = 0;
		dup = true;
		while (cmds[++j])
		{	
			if (ft_strnstr(list->begin->env[i], cmds[j], ft_strlen(cmds[j]))
				&& (ft_strlen_equal(list->begin->env[i]) == ft_strlen(cmds[j])))
				dup = false;
		}
		if (dup == true)
			new_env[k++] = ft_strdup(list->begin->env[i]);
	}
	ft_unset_swap_env(list, new_env, k);
}

void	builtin_unset(t_separate *list, t_data *pipex)
{
	size_t	j;
	size_t	size;
	char	**cmds;

	if (pipex)
		cmds = pipex->cmd;
	else
		cmds = list->cmds;
	if (list->begin->env[0] == NULL)
	{
		j = 1;
		while (cmds[j])
		{
			if (!ft_isalpha(cmds[j][0]))
				g_status = errmsg("unset: ", \
				cmds[j], ": not a valid identifier");
			j++;
		}
		return ;
	}
	size = ft_array_size(list->begin->env) - num_to_unset(list, cmds) + 1;
	ft_unset(list, pipex, cmds, size);
}
