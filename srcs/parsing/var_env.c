/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/11 15:24:26 by jcalon            #+#    #+#             */
/*   Updated: 2022/08/05 17:04:10 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		tmp[k++] = '\"';
		while (env[m])
			tmp[k++] = env[m++];
		tmp[k++] = '\"';
		while (cmd[l])
			tmp[k++] = cmd[l++];
	}
	tmp[k] = '\0';
}

static char	*replace_var_env(t_separate *list, char *cmd, size_t i, size_t j)
{
	size_t	size_var;
	size_t	new_size;
	char	*tmp;
	char	*env;

	env = ft_strdup(ft_get_var_env(list, (cmd + j), (i - j)));
	size_var = ft_strlen(env) - 1;
	new_size = size_var + ft_strlen(cmd) - (i - j) + 3;
	tmp = malloc(sizeof(char) * new_size);
	dup_far_env(cmd, tmp, env, i);
	free(cmd);
	free(env);
	return (tmp);
}

static size_t	str_swap_var_env(t_separate *list, size_t i, size_t j, size_t k)
{
	k = ft_strlen(ft_get_var_env(list, (list->str + j), (i - j)));
	list->str = replace_var_env(list, list->str, i, j);
	return (j + k - 1);
}

static size_t	str_swap_err_num(t_separate *list, size_t j)
{
	list->str = replace_by_code_var_env(list->str);
	return (j + counterr(g_return_code) - 1);
}

static int	check_var_env(t_separate *list, size_t check)
{
	size_t	i;
	size_t	j;
	size_t	k;

	i = check;
	while (list->str[i])
	{
		j = (in_single_quote(list->str, i));
		if (j == ft_strlen(list->str))
			break ;
		else if (i < j)
			i = j;
		if (list->str[i] == '$')
		{
			j = ++i;
			while (list->str[i] && ft_isalnum(list->str[i]))
				i++;
			if (ft_get_var_env(list, (list->str + j), (i - j)))
				return (str_swap_var_env(list, i, j, k));
			else if (list->str[i] && list->str[i] == '?')
				return (str_swap_err_num(list, j));
		}
		i++;
	}
	return (0);
}
