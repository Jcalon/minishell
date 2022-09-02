/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/07 16:26:20 by jcalon            #+#    #+#             */
/*   Updated: 2022/09/02 14:40:57 by jcalon           ###   ########.fr       */
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
		if (ft_isalpha(cmd[i][0])
			&& !check_double_env(list, cmd[i], ft_strlen_equal(cmd[i])))
				count++;
		i++;
	}
	return (count);
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
	if (!new)
	{
		g_status = errmsg("Unexpected malloc error", "", "");
		return (NULL);
	}
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

void	ft_unset_swap_env(t_separate *list, char **new_env, size_t k)
{
	ft_check_dup(list, NULL, k, new_env);
	ft_free_array(list->begin->env);
	list->begin->env = new_env;
}

void	ft_check_dup(t_separate *list, t_data *pipex, size_t k, char **new_env)
{
	new_env[k] = NULL;
	if (k > ft_array_size(new_env))
		ft_error(list, pipex, errmsg("Unexpected malloc error", "", ""));
}
