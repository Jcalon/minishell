/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_out.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/18 14:05:12 by jcalon            #+#    #+#             */
/*   Updated: 2022/08/03 19:39:37 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_fdout(size_t i, t_separate *list, t_data *pipex)
{
	size_t	len;
	size_t	j;
	size_t	k;
	size_t	save;
	size_t	whitespace;
	char	*newline;
	char	*cmd;

	if (pipex)
		cmd = pipex->actual->str;
	else 
		cmd = list->str;
	len = 1;
	save = i;
	whitespace = 0;
	while (ft_iswhitespace(cmd[++i]))
		whitespace++;
	if (cmd[i] == '$')
	{
		g_global.return_code = errmsg(cmd + i, ": ", "ambiguous redirect");
		return (-1);
	}
	while (!ft_iswhitespace(cmd[i]) && !ft_istoken(cmd[i]))
	{
		j = (in_quote(cmd, i));
		if (j == ft_strlen(cmd))
		{
			len += (j - i);
			i = j;
			break;
		}
		else if (i < j)
		{
			len += (j - i);
			i = j;
		}
		else
		{
			i++;
			len++;
		}
	}
	if (list->out != NULL)
	{
		if (list->fdout != -1)
			close(list->fdout);
		free(list->out);
	}
	list->out = malloc(sizeof(char) * len);
	ft_strlcpy(list->out, cmd + save + whitespace + 1, len);
	list->fdout = open(list->out, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (list->fdout == -1)
	{
		g_global.return_code = errmsg(list->out, ": ", strerror(errno));
		return (-1);
	}
	if (pipex && list->fdout != 1)
		pipex->fdout = list->fdout;
	newline = malloc(sizeof(char) * (ft_strlen(cmd) - len - whitespace + 1));
	k = 0;
	while (k < save)
	{
		newline[k] = cmd[k];
		k++;
	}
	ft_strcpy(cmd + save + whitespace + len, newline + k);
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
	return (1);
}

int	get_fdout_append(size_t i, t_separate *list, t_data *pipex)
{
	size_t	len;
	size_t	j;
	size_t	k;
	size_t	save;
	size_t	whitespace;
	char	*newline;
	char	*cmd;

	if (pipex)
		cmd = pipex->actual->str;
	else 
		cmd = list->str;
	len = 1;
	save = i;
	i += 1;
	whitespace = 0;
	while (ft_iswhitespace(cmd[++i]))
		whitespace++;
	if (cmd[i] == '$')
	{
		g_global.return_code = errmsg(cmd + i, ": ", "ambiguous redirect");
		return (-1);
	}
	while (!ft_iswhitespace(cmd[i]) && !ft_istoken(cmd[i]))
	{
		j = (in_quote(cmd, i));
		if (j == ft_strlen(cmd))
		{
			len += (j - i);
			i = j;
			break;
		}
		else if (i < j)
		{
			len += (j - i);
			i = j;
		}
		else
		{
			i++;
			len++;
		}
	}
	if (list->out != NULL)
	{
		if (list->fdout != -1)
			close(list->fdout);
		free(list->out);
	}
	list->out = malloc(sizeof(char) * len);
	ft_strlcpy(list->out, cmd + save + whitespace + 2, len);
	list->fdout = open(list->out, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (list->fdout == -1)
	{
		if (g_global.child_pid)
			g_global.return_code = errmsg(list->out, ": ", strerror(errno));
		return (-1);
	}
	if (pipex && list->fdout != 1)
		pipex->fdout = list->fdout;
	newline = malloc(sizeof(char) * (ft_strlen(cmd) - len - whitespace));
	k = 0;
	while (k < save)
	{
		newline[k] = cmd[k];
		k++;
	}
	ft_strcpy(cmd + save + whitespace + len + 1, newline + k);
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
	return (1);
}