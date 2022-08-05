/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_env_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/05 16:47:47 by jcalon            #+#    #+#             */
/*   Updated: 2022/08/05 16:59:34 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	dup_err_num(char *cmd, char *env, char *tmp)
{
	size_t	i;
	size_t	j;
	size_t	m;

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
}

char	*replace_by_code_var_env(char *cmd)
{
	size_t	size_var;
	size_t	new_size;
	char	*tmp;
	char	*env;

	env = ft_itoa(g_return_code);
	size_var = ft_strlen(env) - 1;
	new_size = size_var + ft_strlen(cmd);
	tmp = malloc(sizeof(char) * new_size);
	dup_err_num(cmd, env, tmp);
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

size_t	in_single_quote(char *str, size_t i)
{
	char	c;

	while (str[i] && (str[i] == '\''))
	{
		c = str[i];
		i++;
		while (str[i] && str[i] != c)
				i++;
		i++;
		break ;
	}
	return (i);
}

void	do_var_env(t_separate *list)
{
	size_t	checkpoint;

	checkpoint = 0;
	while (1)
	{
		checkpoint = check_var_env(list, checkpoint);
		if (!checkpoint)
			break ;
	}
}
