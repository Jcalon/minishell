/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/09 14:00:16 by jcalon            #+#    #+#             */
/*   Updated: 2022/07/11 14:13:27 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**get_path()
{
	char	*path;
	char	**paths;

	path = ft_strdup(ft_getenv("PATH"));
	paths = ft_split(path, ":");
	return (paths);
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
	g_global.env = malloc(sizeof(char *) * (i + 1));
	i = 0;
	while (envp[i])
	{
		g_global.env[i] = ft_strdup(envp[i]);
		i++;
	}
	g_global.env[i] = NULL;
}