/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/09 14:00:16 by jcalon            #+#    #+#             */
/*   Updated: 2022/07/13 12:52:22 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**get_path()
{
	char	*path;
	char	**paths;

	if (g_global.env[0] == NULL)
		return (NULL);
	if (ft_getenv("PATH"))
	{
		path = ft_strdup(ft_getenv("PATH"));
		paths = ft_split(path, ":");
	}
	return (paths);
}

char	*ft_get_var_env(char *str, size_t len)
{
	size_t	i;

	i = 0;
			
	while (g_global.env[i] && !ft_strnstr(g_global.env[i], str, len) && *(g_global.env[i] + len) != '=')
	{
		i++;
	}
	if (g_global.env[i])
		return (g_global.env[i] + len + 1);
	else
		return (NULL);
}

char	*ft_getenv(char *str)
{
	size_t	i;

	i = 0;
	while (g_global.env[i] && !ft_strnstr(g_global.env[i], str, ft_strlen(str)))
		i++;
	return (g_global.env[i]);
}

void	get_env(char **envp)
{
	size_t	i;

	i = 0;
	while (envp[i])
		i++;
	g_global.env = ft_calloc(sizeof(char *), (i + 1));
	i = 0;
	while (envp[i])
	{
		g_global.env[i] = ft_strdup(envp[i]);
		i++;
	}
	g_global.env[i] = NULL;
}