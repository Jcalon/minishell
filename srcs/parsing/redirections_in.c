/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_in.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crazyd <crazyd@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/07 11:39:16 by jcalon            #+#    #+#             */
/*   Updated: 2022/09/01 22:36:16 by crazyd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	remove_redir(t_separate *list, t_data *pipex, size_t *size, char *cmd)
{
	char	*newline;
	size_t	k;

	k = 0;
	newline = malloc(sizeof(char) * (ft_strlen(cmd) - size[0] - size[2] + 1));
	if (!newline)
		ft_error(list, pipex, errmsg("Unexpected malloc error", "", ""));
	while (k < size[1])
	{
		newline[k] = cmd[k];
		k++;
	}
	ft_strcpy(cmd + size[1] + size[2] + size[0], newline + k);
	if (pipex)
	{
		free(pipex->actual->str);
		pipex->actual->str = newline;
	}
	else
	{
		free(list->str);
		list->str = newline;
	}
}

size_t	get_fd_name_len(char *cmd, size_t *size, size_t i)
{
	size_t	j;

	size[0] = 1;
	while (cmd[i] && !ft_iswhitespace(cmd[i]) && !ft_istoken(cmd[i]))
	{
		j = in_quote(cmd, i);
		if (i < j)
		{
			size[0] += (j - i + 1);
			i = j + 1;
		}
		else
		{
			i++;
			size[0]++;
		}
	}
	return (size[0]);
}

static int	open_fdin(t_separate *list, t_data *pipex, char *cmd, size_t *size)
{
	if (list->in != NULL)
	{
		if (list->fdin != -1)
			close(list->fdin);
		free(list->in);
	}
	list->in = malloc(sizeof(char) * size[0]);
	if (!list->in)
		ft_error(list, pipex, errmsg("Unexpected malloc error", "", ""));
	ft_strlcpy(list->in, cmd + size[1] + size[2] + 1, size[0]);
	if (ft_strchr(list->in, '\'') || ft_strchr(list->in, '\"'))
		list->in = cut_quote(list, pipex, list->in);
	list->fdin = open(list->in, O_RDONLY);
	if (list->fdin == -1)
	{
		g_return_code = errmsg(list->in, ": ", strerror(errno));
		return (-1);
	}
	return (0);
}

int	get_fdin(size_t i, t_separate *list, t_data *pipex)
{
	size_t	size[3];
	char	*cmd;

	if (pipex)
		cmd = pipex->actual->str;
	else
		cmd = list->str;
	size[1] = i;
	size[2] = 0;
	while (ft_iswhitespace(cmd[++i]))
		size[2]++;
	if (cmd[i] == '$')
	{
		g_return_code = errmsg(cmd + i, ": ", "ambiguous redirect");
		return (-1);
	}
	size[0] = get_fd_name_len(cmd, size, i);
	if (open_fdin(list, pipex, cmd, size) == -1)
		return (-1);
	if (pipex && list->fdin != 0)
		pipex->fdin = list->fdin;
	remove_redir(list, pipex, size, cmd);
	return (1);
}
