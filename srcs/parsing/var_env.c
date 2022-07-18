/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/11 15:24:26 by jcalon            #+#    #+#             */
/*   Updated: 2022/07/18 17:49:36 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*replace_by_code_var_env(char *cmd)
{
	size_t	size_var;
	size_t	new_size;
	size_t	i;
	size_t	j;
	size_t	m;
	char	*tmp;
	char	*env;

	env = ft_itoa(g_global.return_code);
	size_var = ft_strlen(env) - 1;
	new_size = size_var + ft_strlen(cmd);
	tmp = malloc(sizeof(char) * new_size);
	j = 0;
	i = 0;
	m = 0;
	while (cmd[j])
	{
		while (cmd[j] && cmd[j] != '$')
			tmp[i++] = cmd[j++];
		j += 2;
		while (env[m])
			tmp[i++] = env[m++];
		while (cmd[j] && cmd[j] != '$')
			tmp[i++] = cmd[j++];
	}
	tmp[i] = '\0';
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
	size_t	m;
	char	*tmp;
	char	*env;

	env = ft_strdup(ft_get_var_env((cmd + j), (i - j)));
	size_var = ft_strlen(env) - 1;
	new_size = size_var + ft_strlen(cmd) - (i - j) + 1;
	tmp = malloc(sizeof(char) * new_size);
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
	free(cmd);
	free(env);
	return (tmp);
}

int	counterr(int err)
{
	int	count;

	count = 1;
	while (err > 9)
	{
		count++;
		err /= 10;
	}
	return (count);
}

static int	check_var_env(char	**cmds, size_t index, size_t check)
{
	size_t	i;
	size_t	j;
	size_t	k;

	i = check;
	while (cmds[index][i])
	{
		j = (in_quote(cmds[index], i));
		if (j == ft_strlen(cmds[index]))
			break;
		else if (i < j)
			i = j;
		if (cmds[index][i] == '$')
		{
			j = ++i;
			while (cmds[index][i] && ft_isalnum(cmds[index][i]))
				i++;
			if (ft_get_var_env((cmds[index] + j), (i - j)))
			{
				k = ft_strlen(ft_get_var_env((cmds[index] + j), (i - j)));
				cmds[index] = replace_var_env(cmds[index], i, j);
				return (j + k - 1);
			}
			else if (cmds[index][i] && cmds[index][i] == '?')
			{
				cmds[index] = replace_by_code_var_env(cmds[index]);
				return(j + counterr(g_global.return_code) - 1);
			}
		}
		i++;
	}
	return (0);
}

void	do_var_env(char **cmds)
{
	size_t	i;
	size_t	checkpoint;

	i = 0;
	checkpoint = 0;
	while (i < 1)
	{
		if (ft_strchr(cmds[i], '$'))
		{
			while (1)
			{
				checkpoint = check_var_env(cmds, i, checkpoint);
				if (!checkpoint)
					break;
			}
		}
		i++;
	}
}
