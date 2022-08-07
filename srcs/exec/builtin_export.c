/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/13 11:31:30 by jcalon            #+#    #+#             */
/*   Updated: 2022/08/07 17:14:20 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	do_update(t_separate *list, char **cmd, size_t i, size_t j)
{
	if (!ft_strncmp(list->begin->env[j], \
		cmd[i], ft_strlen_equal(cmd[i]))
		&& ft_strlen_equal(list->begin->env[j]) \
		== ft_strlen_equal(cmd[i])
		&& cmd[i][ft_strlen_equal(cmd[i]) + 1] == '='
		&& cmd[i][ft_strlen_equal(cmd[i]) + 2] != '\0'
		&& cmd[i][ft_strlen_equal(cmd[i])] == '+')
	{
		ft_join_more(&list->begin->env[j], \
		(cmd[i] + ft_strlen_equal(cmd[i]) + 2));
	}
	else if (!ft_strncmp(list->begin->env[j], \
		cmd[i], ft_strlen_equal(cmd[i]))
		&& ft_strlen_equal(list->begin->env[j]) \
		== ft_strlen_equal(cmd[i])
		&& cmd[i][ft_strlen_equal(cmd[i])] == '='
		&& cmd[i][ft_strlen_equal(cmd[i]) + 1] != '\0')
	{
		free(list->begin->env[j]);
		list->begin->env[j] = ft_strdup(cmd[i]);
	}
}

static void	update_double(t_separate *list, char **cmd, bool update)
{
	size_t	i;
	size_t	j;

	if (update == true)
	{
		j = 0;
		while (list->begin->env[j])
		{
			i = 1;
			while (cmd[i])
			{
				do_update(list, cmd, i, j);
				i++;
			}
			j++;
		}
	}
}

static int	malloc_new_env(t_separate *list, char **cmds, char **new)
{
	size_t	size;
	size_t	i;
	char	**new_env;

	i = 0;
	if (list->begin->env[0] == NULL)
		size = num_to_export(list, cmds) + 1;
	else
		size = num_to_export(list, cmds) \
		+ ft_array_size(list->begin->env) + 1;
	new_env = malloc(sizeof(char *) * size);
	ft_free_array(new);
	new = new_env;
	while (list->begin->env[i])
	{
		new[i] = ft_strdup(list->begin->env[i]);
		i++;
	}
	return (i);
}

static void	make_new_env(t_separate *list, char **cmd, char **new, size_t i)
{
	size_t	j;
	bool	update;

	update = true;
	j = 0;
	while (cmd[++j])
	{
		if (!ft_isalpha(cmd[j][0]))
			g_return_code = errmsg("export: ", \
			cmd[j], ": not a valid identifier");
		else if (!check_double_env(list, cmd[j], ft_strlen_equal(cmd[j])))
		{
			if (!ft_strnstr(cmd[j], "+", ft_strlen_equal(cmd[j]) + 1))
				new[i++] = ft_strdup(cmd[j]);
			else
			{
				new[i++] = concat_new(cmd[j]);
				update = false;
			}
		}
	}
	new[i] = NULL;
	ft_free_array(list->begin->env);
	list->begin->env = new;
	update_double(list, cmd, update);
}

void	builtin_export(t_separate *list, t_data *pipex)
{
	size_t	i;
	char	**new_env;
	char	**cmds;

	i = 0;
	new_env = ft_calloc(1, sizeof(char *));
	if (pipex)
		cmds = pipex->cmd;
	else
		cmds = list->cmds;
	g_return_code = 0;
	if (!cmds[1])
		builtin_env(true, list);
	else
	{
		i = malloc_new_env(list, cmds, new_env);
		make_new_env(list, cmds, new_env, i);
	}
}
