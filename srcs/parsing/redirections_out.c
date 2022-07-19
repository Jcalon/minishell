/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_out.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/18 14:05:12 by jcalon            #+#    #+#             */
/*   Updated: 2022/07/19 12:35:43 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_fdout(char **cmds, size_t ind, size_t i, t_separate *list, t_data *pipex)
{
	size_t	len;
	size_t	j;
	size_t	k;
	size_t	save;
	size_t	whitespace;
	char	*newline;

	len = 1;
	save = i;
	whitespace = 0;
	while (ft_iswhitespace(cmds[ind][++i]))
		whitespace++;
	if (cmds[ind][i] == '$')
	{
		g_global.return_code = errmsg(cmds[ind] + i, ": ", "ambiguous redirect");
		return (-1);
	}
	while (!ft_iswhitespace(cmds[ind][i]) && !ft_istoken(cmds[ind][i]))
	{
		j = (in_quote(cmds[ind], i));
		if (j == ft_strlen(cmds[ind]))
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
	if (list->out == NULL)
		list->out = calloc(sizeof(char *), 2);
	if (list->out[0] != NULL)
	{
		if (list->fdout != -1)
			close(list->fdout);
		free(list->out[0]);
	}
	list->out[0] = malloc(sizeof(char) * len);
	ft_strlcpy(list->out[0], cmds[ind] + save + whitespace + 1, len);
	list->fdout = open(list->out[0], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (list->fdout == -1)
	{
		g_global.return_code = errmsg(list->out[0], ": ", strerror(errno));
		return (-1);
	}
	if (pipex && list->fdout != 1)
		pipex->fdout = list->fdout;
	newline = malloc(sizeof(char) * (ft_strlen(cmds[ind]) - len - whitespace + 1));
	k = 0;
	while (k < save)
	{
		newline[k] = cmds[ind][k];
		k++;
	}
	ft_strcpy(cmds[ind] + save + whitespace + len, newline + k);
	free (cmds[ind]);
	cmds[ind] = newline;
	return (1);
}

int	get_fdout_append(char **cmds, size_t ind, size_t i, t_separate *list, t_data *pipex)
{
	size_t	len;
	size_t	j;
	size_t	k;
	size_t	save;
	size_t	whitespace;
	char	*newline;

	len = 1;
	save = i;
	i += 1;
	whitespace = 0;
	while (ft_iswhitespace(cmds[ind][++i]))
		whitespace++;
	if (cmds[ind][i] == '$')
	{
		g_global.return_code = errmsg(cmds[ind] + i, ": ", "ambiguous redirect");
		return (-1);
	}
	while (!ft_iswhitespace(cmds[ind][i]) && !ft_istoken(cmds[ind][i]))
	{
		j = (in_quote(cmds[ind], i));
		if (j == ft_strlen(cmds[ind]))
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
	if (list->out == NULL)
		list->out = calloc(sizeof(char *), 2);
	if (list->out[0] != NULL)
	{
		if (list->fdout != -1)
			close(list->fdout);
		free(list->out[0]);
	}
	list->out[0] = malloc(sizeof(char) * len);
	ft_strlcpy(list->out[0], cmds[ind] + save + whitespace + 2, len);
	list->fdout = open(list->out[0], O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (list->fdout == -1)
	{
		g_global.return_code = errmsg(list->out[0], ": ", strerror(errno));
		return (-1);
	}
	if (pipex && list->fdout != 1)
		pipex->fdout = list->fdout;
	newline = malloc(sizeof(char) * (ft_strlen(cmds[ind]) - len - whitespace));
	k = 0;
	while (k < save)
	{
		newline[k] = cmds[ind][k];
		k++;
	}
	ft_strcpy(cmds[ind] + save + whitespace + len + 1, newline + k);
	free (cmds[ind]);
	cmds[ind] = newline;
	return (1);
}