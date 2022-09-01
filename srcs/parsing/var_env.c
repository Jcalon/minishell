/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmattera <nmattera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/11 15:24:26 by jcalon            #+#    #+#             */
/*   Updated: 2022/09/01 13:54:06 by nmattera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Tant qu'il y a des $ qui ne sont pas dans des '' on expand avec env
	si l'expand est invalid on le tej et si $? on remplace par g_return_code */

static void	dup_var_env(char *cmd, char *tmp, char *env, size_t i)
{
	size_t	k;
	size_t	l;
	size_t	m;

	l = 0;
	k = 0;
	m = 0;
	while (cmd[l])
	{
		while (cmd[l] && cmd[l] != '$')
			tmp[k++] = cmd[l++];
		l = i;
		while (env[m])
			tmp[k++] = env[m++];
		while (cmd[l])
			tmp[k++] = cmd[l++];
	}
	tmp[k] = '\0';
}

static char	*r_var_env(t_separate *list, t_data *pipex, char *cmd, size_t i[2])
{
	size_t	size_var;
	size_t	new_size;
	char	*tmp;
	char	*env;

	env = ft_strdup(ft_get_var_env(list, (cmd + i[1]), (i[0] - i[1])));
	if (!env)
		ft_error(list, pipex, errmsg("Unexpected malloc error", "", ""));
	size_var = ft_strlen(env) - 1;
	new_size = size_var + ft_strlen(cmd) - (i[0] - i[1]) + 1;
	tmp = malloc(sizeof(char) * new_size);
	if (!tmp)
	{
		free(env);
		free(cmd);
		return (NULL);
	}
	dup_var_env(cmd, tmp, env, i[0]);
	free(cmd);
	free(env);
	return (tmp);
}

static size_t	str_var_env(t_separate *list, t_data *pipex, size_t i[2])
{
	size_t	k;
	char	*cmd;

	if (pipex)
		cmd = pipex->actual->str;
	else
		cmd = list->str;
	k = ft_strlen(ft_get_var_env(list, (cmd + i[1]), (i[0] - i[1])));
	cmd = r_var_env(list, pipex, cmd, i);
	if (!cmd)
		ft_error(list, pipex, errmsg("Unexpected malloc error", "", ""));
	if (pipex)
		pipex->actual->str = cmd;
	else
		list->str = cmd;
	return (i[1] + k - 1);
}

static size_t	str_swap_err_num(t_separate *list, t_data *pipex, size_t j)
{
	char	*cmd;

	if (pipex)
		cmd = pipex->actual->str;
	else
		cmd = list->str;
	cmd = replace_by_code_var_env(cmd);
	if (!cmd)
		ft_error(list, pipex, errmsg("Unexpected malloc error", "", ""));
	if (pipex)
		pipex->actual->str = cmd;
	else
		list->str = cmd;
	return (j + counterr(g_return_code) - 1);
}

int	check_var_env(t_separate *list, t_data *pipex, size_t check)
{
	size_t	i[2];
	char	*cmd;

	cmd = ft_test_pipe(list, pipex);
	i[0] = check;
	while (cmd[i[0]])
	{
		i[1] = (in_single_quote(cmd, i[0]));
		if (i[0] < i[1])
			i[0] = i[1];
		if (cmd[i[0]] && cmd[i[0]] == '$')
		{
			i[1] = ++i[0];
			while (cmd[i[0]] && ft_isalnum(cmd[i[0]]))
				i[0]++;
			if (ft_get_var_env(list, (cmd + i[1]), (i[0] - i[1])))
				return (str_var_env(list, pipex, i));
			else if (cmd[i[0]] && cmd[i[0]] == '?')
				return (str_swap_err_num(list, pipex, i[1]));
		}
		if (cmd[i[0]])
			i[0]++;
	}
	return (0);
}
