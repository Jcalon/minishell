/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/09 14:00:16 by jcalon            #+#    #+#             */
/*   Updated: 2022/09/02 14:20:07 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**get_path(t_separate *list)
{
	char	*path;
	char	**paths;

	if (list->begin->env[0] == NULL)
		return (NULL);
	if (ft_getenv(list, "PATH"))
	{
		path = ft_strdup(ft_getenv(list, "PATH") + 5);
		if (!path)
			ft_error(list, NULL, errmsg("Unexpected malloc error", "", ""));
		paths = ft_split(path, ":");
		free(path);
	}
	else
		return (NULL);
	return (paths);
}

char	*ft_get_var_env(t_separate *list, char *str, size_t len)
{
	size_t	i;

	i = 0;
	while (list->begin->env[i])
	{
		if (ft_strnstr(list->begin->env[i], str, len))
			if (*(list->begin->env[i] + len) == '=')
				break ;
		i++;
	}
	if (list->begin->env[i])
		return (list->begin->env[i] + len + 1);
	else
		return (NULL);
}

char	*ft_getenv(t_separate *list, char *str)
{
	size_t	i;

	i = 0;
	while (list->begin->env[i]
		&& ft_strncmp(list->begin->env[i], str, ft_strlen(str)))
		i++;
	if (!list->begin->env[i])
		return (NULL);
	return (list->begin->env[i]);
}

char	*ft_shlvl(t_separate *list, char *str)
{
	char	*shlvl;
	char	*val_up;
	int		value;

	shlvl = ft_strdup("SHLVL=");
	if (!shlvl)
		ft_error(list, NULL, errmsg("Unexpected malloc error", "", ""));
	value = ft_atoi(str + 6);
	value++;
	val_up = ft_itoa(value);
	if (!val_up)
		ft_error(list, NULL, errmsg("Unexpected malloc error", "", ""));
	ft_join_more(&shlvl, val_up);
	if (!shlvl)
		ft_error(list, NULL, errmsg("Unexpected malloc error", "", ""));
	free(val_up);
	return (shlvl);
}

void	get_env(t_separate *list, char **envp)
{
	size_t	i;

	i = 0;
	list->begin = NULL;
	while (envp[i])
		i++;
	list->env = ft_calloc(sizeof(char *), (i + 1));
	if (!list->env)
		ft_error(list, NULL, errmsg("Unexpected malloc error", "", ""));
	i = 0;
	while (envp[i])
	{
		if (ft_strnstr(envp[i], "SHLVL=", 6))
			list->env[i] = ft_shlvl(list, envp[i]);
		else
			list->env[i] = ft_strdup(envp[i]);
		if (!list->env[i])
			ft_error(list, NULL, errmsg("Unexpected malloc error", "", ""));
		i++;
	}
	list->env[i] = NULL;
}
