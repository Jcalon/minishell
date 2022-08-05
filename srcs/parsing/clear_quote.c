/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clear_quote.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/11 17:54:19 by jcalon            #+#    #+#             */
/*   Updated: 2022/08/05 15:41:07 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_quote(char *str)
{
	int		count;
	size_t	i;
	size_t	k;

	count = 0;
	i = 0;
	while (str[i])
	{
		k = (in_quote((char *)str, i));
		if (k == ft_strlen(str))
		{
			count += 2;
			i = k;
			break ;
		}
		else if (i == k)
			i++;
		else if (i < k)
		{
			count += 2;
			i = k;
		}
	}
	return (count);
}

static char	*cut_quote(char	*cmd)
{
	size_t	i;
	size_t	j;
	char	*dequoted;

	dequoted = malloc(sizeof(char) * (ft_strlen(cmd) - count_quote(cmd) + 1));
	i = 0;
	j = 0;
	while (cmd[i] != '\0')
	{
		if (cmd[i] == '\'' || cmd[i] == '\"')
			i++;
		else
			dequoted[j++] = cmd[i++];
	}
	dequoted[j] = '\0';
	free(cmd);
	return (dequoted);
}

static void	clear_pipex(t_data *pipex)
{
	size_t	i;

	i = 0;
	while (pipex->cmd[i])
	{
		if (ft_strchr(pipex->cmd[i], '\'') || ft_strchr(pipex->cmd[i], '\"'))
			pipex->cmd[i] = cut_quote(pipex->cmd[i]);
		i++;
	}	
}

static void	clear_standard(t_separate *list)
{
	size_t	i;

	i = 0;
	while (list->cmds[i])
	{
		if (ft_strchr(list->cmds[i], '\'') || ft_strchr(list->cmds[i], '\"'))
			list->cmds[i] = cut_quote(list->cmds[i]);
		i++;
	}
}

void	clear_quote(t_separate *list, t_data *pipex)
{
	size_t	i;

	i = 0;
	if (pipex)
		clear_pipex(pipex);
	else
		clear_standard(list);
	if (list->in)
	{
		if (ft_strchr(list->in, '\'') || ft_strchr(list->in, '\"'))
			list->in = cut_quote(list->in);
	}
	if (list->out)
	{
		if (ft_strchr(list->out, '\'') || ft_strchr(list->out, '\"'))
			list->out = cut_quote(list->out);
	}
}
