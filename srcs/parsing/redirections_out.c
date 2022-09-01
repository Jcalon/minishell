/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_out.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmattera <nmattera@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/18 14:05:12 by jcalon            #+#    #+#             */
/*   Updated: 2022/09/01 13:57:58 by nmattera         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	open_fdout(t_separate *list, t_data *pipex, char *cmd, size_t *size)
{
	if (list->out != NULL)
	{
		if (list->fdout != -1)
			close(list->fdout);
		free(list->out);
	}
	list->out = malloc(sizeof(char) * size[0]);
	if (!list->out)
		ft_error(list, pipex, errmsg("Unexpected malloc error", "", ""));
	ft_strlcpy(list->out, cmd + size[1] + size[2] + 1, size[0]);
	if (size[3] == 0)
		list->fdout = open(list->out, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (size[3] == 1)
		list->fdout = open(list->out, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (list->fdout == -1)
	{
		g_return_code = errmsg(list->out, ": ", strerror(errno));
		return (-1);
	}
	return (0);
}

int	get_fdout(size_t i, t_separate *list, t_data *pipex)
{
	size_t	size[4];
	char	*cmd;

	if (pipex)
		cmd = pipex->actual->str;
	else
		cmd = list->str;
	size[1] = i;
	size[2] = 0;
	size[3] = 0;
	while (ft_iswhitespace(cmd[++i]))
		size[2]++;
	if (cmd[i] == '$')
	{
		g_return_code = errmsg(cmd + i, ": ", "ambiguous redirect");
		return (-1);
	}
	size[0] = get_fd_name_len(cmd, size, i);
	if (open_fdout(list, pipex, cmd, size) == -1)
		return (-1);
	if (pipex && list->fdout != 1)
		pipex->fdout = list->fdout;
	remove_redir(list, pipex, size, cmd);
	return (1);
}

int	get_fdout_append(size_t i, t_separate *list, t_data *pipex)
{
	size_t	size[4];
	char	*cmd;

	if (pipex)
		cmd = pipex->actual->str;
	else
		cmd = list->str;
	size[1] = i;
	size[2] = 1;
	size[3] = 1;
	i += 1;
	while (ft_iswhitespace(cmd[++i]))
		size[2]++;
	if (cmd[i] == '$')
	{
		g_return_code = errmsg(cmd + i, ": ", "ambiguous redirect");
		return (-1);
	}
	size[0] = get_fd_name_len(cmd, size, i);
	if (open_fdout(list, pipex, cmd, size) == -1)
		return (-1);
	if (pipex && list->fdout != 1)
		pipex->fdout = list->fdout;
	remove_redir(list, pipex, size, cmd);
	return (1);
}

/* Rien a voir avec out mais pour gagner de la place */

char	*ft_test_pipe(t_separate *list, t_data *pipex)
{
	if (pipex)
		return (pipex->actual->str);
	else
		return (list->str);
}
