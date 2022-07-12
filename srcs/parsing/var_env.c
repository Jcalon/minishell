/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/11 15:24:26 by jcalon            #+#    #+#             */
/*   Updated: 2022/07/12 14:13:21 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*replace_by_code_var_env(char *cmd)
{
	size_t	size_var;
	size_t	new_size;
	size_t	i;
	size_t	j;
	char	*tmp;
	char	*env;

	env = ft_itoa(g_global.return_code);
	size_var = ft_strlen(env) - 1;
	new_size = size_var + ft_strlen(cmd) - 2;
	tmp = malloc(sizeof(char) * new_size);
	j = 0;
	i = 0;
	while (cmd[j])
	{
		while (cmd[j] && cmd[j] != '$')
			tmp[i++] = cmd[j++];
		j += 2;
		while (*env)
			tmp[i++] = *env++;
		while (cmd[j] && cmd[j] != '$')
			tmp[i++] = cmd[j++];
	}
	free(cmd);
	free(env);
	return (tmp);
}

static char	*replace_var_env(char *cmd, size_t i, size_t j)
{
	size_t	size_var;
	size_t	new_size;
	size_t	k;
	size_t	l;
	char	*tmp;
	char	*env;

	env = ft_strdup(ft_get_var_env((cmd + j), (i - j)));
	size_var = ft_strlen(env) - 1;
	new_size = size_var + ft_strlen(cmd) - (i - j) - 1;
	tmp = malloc(sizeof(char) * new_size);
	l = 0;
	k = 0;
	while (cmd[l])
	{
		while (cmd[l] && cmd[l] != '$')
			tmp[k++] = cmd[l++];
		l += (i - j);
		while (*env)
			tmp[k++] = *env++;
		while (cmd[l] && cmd[l] != '$')
			tmp[k++] = cmd[l++];
	}
	free(cmd);
	free(env);
	return (tmp);
}

static int	check_var_env(char	*cmd)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (cmd[i])
	{
		j = (in_quote(cmd, i));
		if (j == ft_strlen(cmd))
			break;
		else if (i < j)
			i = j;
		if (cmd[i] == '$')
		{
			j = ++i;
			while (cmd[i] && ft_isalnum(cmd[i]))
				i++;
			if (ft_get_var_env((cmd + j), (i - j)))
			{
				cmd = replace_var_env(cmd, i, j);
				return (1);
			}
			else if (cmd[i] && cmd[i] == '?')
			{
				cmd = replace_by_code_var_env(cmd);
				return (1);
			}
		}
		i++;
	}
	return (0);
}

void	do_var_env(char **cmds)
{
	size_t	i;

	i = 0;
	while (cmds[i])
	{
		if (ft_strchr(cmds[i], '$'))
		{
			while (1)
			{
				if (!check_var_env(cmds[i]))
					break;
			}
		}
		i++;
	}
}
