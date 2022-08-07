/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_heredoc.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/19 10:49:58 by jcalon            #+#    #+#             */
/*   Updated: 2022/08/07 19:07:42 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	open_here(t_separate *list, t_data *pipex, char *cmd, size_t *size)
{
	if (list->in != NULL)
	{
		if (list->fdin != -1)
			close(list->fdin);
		free(list->in);
	}
	list->in = malloc(sizeof(char) * size[0]);
	ft_strlcpy(list->in, cmd + size[2] + size[1] + 1, size[0]);
	remove_redir(list, pipex, size, cmd);
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
	return (0);
}

static void	get_heredoc_lines(t_separate *list)
{
	char	*line;
	void	*ctl;

	ctl = rl_getc_function;
	rl_getc_function = getc;
	g_return_code = 0;
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
}

int	get_heredoc(size_t i, t_separate *list, t_data *pipex)
{
	size_t	size[3];
	char	*cmd;

	if (pipex)
		cmd = pipex->actual->str;
	else
		cmd = list->str;
	size[1] = i;
	size[2] = 1;
	i += 1;
	while (ft_iswhitespace(cmd[++i]))
		size[2]++;
	size[0] = get_fd_name_len(cmd, size, i);
	if (open_here(list, pipex, cmd, size) == -1)
		return (-1);
	get_heredoc_lines(list);
	return (1);
}
