/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_heredoc.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crazyd <crazyd@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/19 10:49:58 by jcalon            #+#    #+#             */
/*   Updated: 2022/07/21 09:11:48 by crazyd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_heredoc(size_t i, t_separate *list, t_data *pipex)
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
	while (ft_iswhitespace(list->str[++i]))
		whitespace++;
	while (!ft_iswhitespace(list->str[i]) && !ft_istoken(list->str[i]))
	{
		j = (in_quote(list->str, i));
		if (j == ft_strlen(list->str))
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
	if (list->in != NULL)
	{
		if (list->fdin != -1)
			close(list->fdin);
		free(list->in);
	}
	list->in = malloc(sizeof(char) * len);
	ft_strlcpy(list->in, list->str + save + whitespace + 2, len);
	newline = malloc(sizeof(char) * (ft_strlen(list->str) - len - whitespace + 1));
	k = 0;
	while (k < save)
	{
		newline[k] = list->str[k];
		k++;
	}
	ft_strcpy(list->str + save + whitespace + len + 1, newline + k);
	free (list->str);
	list->str = newline;
	list->fdin = open(".heredoc.tmp", O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (list->fdin == -1)
	{
		g_global.return_code = errmsg(list->in, ": ", strerror(errno));
		return (-1);
	}
	list->heredoc = 1;
	if (pipex && list->fdin != 0)
	{
		pipex->fdin = list->fdin;
		pipex->heredoc = list->heredoc;	
	}
	line = "";
	// signal(SIGINT, handle_heredoc);
	while (1)
	{
		ft_putstr_fd("> ", 1);
		line = get_next_line(STDIN_FILENO);
		if (line == NULL)
			break ;
		if (ft_strlen(list->in) + 1 == ft_strlen(line)
			&& !ft_strncmp(line, list->in, ft_strlen(list->in + 1)))
		{
			free(line);
			break ;
		}
		else
			ft_putstr_fd(line, list->fdin);
		free(line);
	}
	close(list->fdin);
	// if (g_global.return_code == 130)
	// 		return (-1);
	return (1);
}