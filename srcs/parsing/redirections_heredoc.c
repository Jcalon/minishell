/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_heredoc.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/19 10:49:58 by jcalon            #+#    #+#             */
/*   Updated: 2022/08/05 15:11:42 by jcalon           ###   ########.fr       */
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
	if (list->in != NULL)
	{
		if (list->fdin != -1)
			close(list->fdin);
		free(list->in);
	}
	list->in = malloc(sizeof(char) * len);
	ft_strlcpy(list->in, cmd + save + whitespace + 2, len);
	newline = malloc(sizeof(char) * (ft_strlen(cmd) - len - whitespace + 1));
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
	list->fdin = open(".heredoc.tmp", O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (list->fdin == -1)
	{
		g_return_code = errmsg(list->in, ": ", strerror(errno));
		return (-1);
	}
	list->heredoc = 1;
	if (pipex && list->fdin != 0)
	{
		pipex->fdin = list->fdin;
		pipex->heredoc = list->heredoc;	
	}
 	void    *ctl;

    ctl = rl_getc_function;
    rl_getc_function = getc;
	while (1)
	{
		line = readline("> ");
		if (g_return_code == 130)
			break ;
		if (line == NULL)
			break ;
		if (ft_strlen(list->in) == ft_strlen(line)
			&& !ft_strncmp(line, list->in, ft_strlen(list->in)))
		{
			free(line);
			break ;
		}
		else
			ft_putendl_fd(line, list->fdin);
		free(line);
	}
    rl_getc_function = ctl;
	close(list->fdin);
	return (1);
}
