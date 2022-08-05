/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/13 11:31:30 by jcalon            #+#    #+#             */
/*   Updated: 2022/08/05 15:10:20 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	num_to_export(t_separate *list, char **cmd)
{
	size_t	i;
	size_t	count;

	i = 1;
	count = 0;
	while (cmd[i])
	{
		if (ft_isalpha(cmd[i][0]) && !check_double_env(list, cmd[i], ft_strlen_equal(cmd[i])))
				count++;
		i++;
	}
	return (count);
}

void	update_double(t_separate *list, char **cmd)
{
	size_t	i;
	size_t	j;

	j = 0;
	while (list->begin->env[j])
	{
		i = 1;
		while (cmd[i])
		{
			if (!ft_strncmp(list->begin->env[j], cmd[i], ft_strlen_equal(cmd[i]))
				&& ft_strlen_equal(list->begin->env[j]) == ft_strlen_equal(cmd[i])
					&& cmd[i][ft_strlen_equal(cmd[i]) + 1] == '='
						&& cmd[i][ft_strlen_equal(cmd[i]) + 2] != '\0'
							&& cmd[i][ft_strlen_equal(cmd[i])] == '+')
			{
				ft_join_more(&list->begin->env[j], (cmd[i] + ft_strlen_equal(cmd[i]) + 2));
			}
			else if (!ft_strncmp(list->begin->env[j], cmd[i], ft_strlen_equal(cmd[i]))
				&& ft_strlen_equal(list->begin->env[j]) == ft_strlen_equal(cmd[i])
					&& cmd[i][ft_strlen_equal(cmd[i])] == '='
						&& cmd[i][ft_strlen_equal(cmd[i]) + 1] != '\0')
			{
				free(list->begin->env[j]);
				list->begin->env[j] = ft_strdup(cmd[i]);
			}
			i++;
		}
		j++;
	}
}

char	*concat_new(char *cmd)
{
	size_t	i;
	size_t	j;
	char	*new;
	int		plus;

	i = 0;
	j = 0;
	plus = 0;
	new = malloc(sizeof(char) * ft_strlen(cmd));
	while (cmd[i])
	{
		if (cmd[i] == '+' && plus == 0)
		{
			plus++;
			i++;
		}
		new[j++] = cmd[i++];
	}
	new[j] = '\0';
	return (new);
}

int	builtin_export(t_separate *list, t_data *pipex)
{
	size_t	i;
	size_t	j;
	size_t	size;
	int		err;
	bool	update;
	char	**new_env;
	char	**cmds;

	if (pipex)
		cmds = pipex->cmd;
	else 
		cmds = list->cmds;
	err = 0;
	update = true;
	if (!cmds[1])
		builtin_env(true, list);
	else
	{
		i = 0;
		if (list->begin->env[0] == NULL)
			size = num_to_export(list, cmds) + 1;
		else
			size = num_to_export(list, cmds) + ft_array_size(list->begin->env) + 1;
		new_env = malloc(sizeof(char *) * size);
		while (list->begin->env[i])
		{
			new_env[i] = ft_strdup(list->begin->env[i]);
			i++;
		}
		j = 1;
		while (cmds[j])
		{
			if (!ft_isalpha(cmds[j][0]))
				err = errmsg("export: ", cmds[j], ": not a valid identifier");	
			else if (!check_double_env(list, cmds[j], ft_strlen_equal(cmds[j])))
			{
				if (!ft_strnstr(cmds[j], "+", ft_strlen_equal(cmds[j]) + 1))
					new_env[i++] = ft_strdup(cmds[j]);
				else
				{
					new_env[i++] = concat_new(cmds[j]);
					update = false;
				}
			}
			j++;
		}
		new_env[i] = NULL;
		ft_free_array(list->begin->env);
		list->begin->env = new_env;
	}
	if (update == true)
		update_double(list, cmds);
	g_return_code = err;
	return (0);
}
