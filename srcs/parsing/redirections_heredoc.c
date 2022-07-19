/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_heredoc.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/19 10:49:58 by jcalon            #+#    #+#             */
/*   Updated: 2022/07/19 12:59:51 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_heredoc(char **cmds, size_t ind, size_t i, t_separate *list, t_data *pipex)
{
	char	*line;
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
	if (list->in == NULL)
		list->in = calloc(sizeof(char *), 2);
	if (list->in[0] != NULL)
	{
		if (list->fdin != -1)
			close(list->fdin);
		free(list->in[0]);
	}
	list->in[0] = malloc(sizeof(char) * len);
	ft_strlcpy(list->in[0], cmds[ind] + save + whitespace + 2, len);
	newline = malloc(sizeof(char) * (ft_strlen(cmds[ind]) - len - whitespace + 1));
	k = 0;
	while (k < save)
	{
		newline[k] = cmds[ind][k];
		k++;
	}
	ft_strcpy(cmds[ind] + save + whitespace + len + 1, newline + k);
	free (cmds[ind]);
	cmds[ind] = newline;
	list->fdin = open(".heredoc.tmp", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (list->fdin == -1)
	{
		g_global.return_code = errmsg(list->in[0], ": ", strerror(errno));
		return (-1);
	}
	list->heredoc = 1;
	if (pipex && list->fdin != 0)
	{
		pipex->fdin = list->fdin;
		printf("here %d", pipex->heredoc);
		pipex->heredoc = list->heredoc;	
	}
	line = "";
	while (1)
	{
		ft_putstr_fd("> ", 1);
		line = get_next_line(STDIN_FILENO);
		if (line == NULL)
			break ;
		if (ft_strlen(list->in[0]) + 1 == ft_strlen(line)
			&& !ft_strncmp(line, list->in[0], ft_strlen(list->in[0] + 1)))
		{
			free(line);
			break ;
		}
		else
			ft_putstr_fd(line, list->fdin);
		free(line);
	}
	close(list->fdin);
	return (1);
}