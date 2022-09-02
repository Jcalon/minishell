/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clear_quote.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/11 17:54:19 by jcalon            #+#    #+#             */
/*   Updated: 2022/09/02 15:05:49 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* On vire les quotes de notre array de cmds */

static int	count_quote(char *str)
{
	int		count;
	size_t	i;
	size_t	k;

	count = 0;
	i = 0;
	while (str[i])
	{
		k = in_quote((char *)str, i);
		if (i < k && k != i + 1)
		{
			count += 2;
			i = k + 1;
		}
		if (i == k || k == i + 1)
			i++;
	}
	return (count);
}

static int	test_quote(char *cmd, size_t i, char c)
{
	if (((cmd[i] == '\'' && cmd[i + 1] != '\'')
			|| (cmd[i] == '"' && cmd[i + 1] != '"'))
		&& c == 0 && cmd[i + 1] != '\0')
		return (1);
	return (0);
}

char	*cut_quote(t_separate *list, t_data *pipex, char *cmd)
{
	size_t	i;
	size_t	j;
	char	c;
	char	*dequoted;

	dequoted = malloc(sizeof(char) * (ft_strlen(cmd) - count_quote(cmd) + 1));
	if (!dequoted)
		ft_error(list, pipex, errmsg("Unexpected malloc error", "", ""));
	i = 0;
	j = 0;
	c = 0;
	while (cmd[i] != '\0')
	{
		if (test_quote(cmd, i, c))
			c = cmd[i++];
		dequoted[j++] = cmd[i++];
		if (cmd[i] && cmd[i] == c && c != 0)
		{
			c = 0;
			i++;
		}
	}
	dequoted[j] = '\0';
	return (free(cmd), dequoted);
}

static void	clear_list_str(t_separate *list)
{
	size_t	i;

	i = 0;
	while (list->cmds[i])
	{
		if (ft_strchr(list->cmds[i], '\'')
			|| ft_strchr(list->cmds[i], '\"'))
			list->cmds[i] = cut_quote(list, NULL, list->cmds[i]);
		if (!ft_strcmp(list->cmds[0], "echo"))
			break ;
		i++;
	}
}

void	clear_quote(t_separate *list, t_data *pipex)
{
	size_t	i;

	i = 0;
	if (pipex)
	{
		while (pipex->cmd[i])
		{
			if (ft_strchr(pipex->cmd[i], '\'')
				|| ft_strchr(pipex->cmd[i], '\"'))
				pipex->cmd[i] = cut_quote(list, pipex, pipex->cmd[i]);
			if (!ft_strcmp(pipex->cmd[0], "echo"))
				break ;
			i++;
		}
	}
	else
		clear_list_str(list);
}
