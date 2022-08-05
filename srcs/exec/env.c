/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/09 14:00:16 by jcalon            #+#    #+#             */
/*   Updated: 2022/08/05 14:28:15 by jcalon           ###   ########.fr       */
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
		path = ft_strdup(ft_getenv(list, "PATH"));
		paths = ft_split(path, ":");
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
	while (list->begin->env[i] && ft_strncmp(list->begin->env[i], str, ft_strlen(str)))
		i++;
	if (!list->begin->env[i])
		return (NULL);
	return (list->begin->env[i]);
}

void	get_env(t_separate *list, char **envp)
{
	size_t	i;

	i = 0;
	while (envp[i])
		i++;
	list->env = ft_calloc(sizeof(char *), (i + 1));
	i = 0;
	while (envp[i])
	{
		list->env[i] = ft_strdup(envp[i]);
		i++;
	}
	list->env[i] = NULL;
}