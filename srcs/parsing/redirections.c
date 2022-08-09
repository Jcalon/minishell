/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/15 15:17:18 by jcalon            #+#    #+#             */
/*   Updated: 2022/08/09 16:14:19 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Tant qu'on trouve des redirs dans la ligne de cmds
	on les trim et on les dup dans list->out ou in
	on check les 4 cas < << >> > sachant que 
	si plusieurs redir on lit ou écrit dans la derniere */

int	ft_istoken(int c)
{
	if (c != '<' && c != '>' && c != '|' && c != ';')
		return (0);
	return (1);
}

static int	get_fd(t_separate *list, t_data *pipex, size_t i, char *cmd)
{
	if (cmd[i] == '<' && cmd[i + 1] == '<')
	{
		if (get_heredoc(i, list, pipex) == -1)
			return (-1);
		return (1);
	}
	else if (cmd[i] == '<')
	{
		if (get_fdin(i, list, pipex) == -1)
			return (-1);
		return (1);
	}
	else if (cmd[i] == '>' && list->str[i + 1] == '>')
	{
		if (get_fdout_append(i, list, pipex) == -1)
			return (-1);
		return (1);
	}
	else if (cmd[i] == '>')
	{
		if (get_fdout(i, list, pipex) == -1)
			return (-1);
		return (1);
	}
	return (0);
}

static int	check_fd(t_separate *list, t_data *pipex)
{
	size_t	i;
	size_t	j;
	char	*cmd;

	i = 0;
	if (pipex)
		cmd = pipex->actual->str;
	else
		cmd = list->str;
	while (cmd[i])
	{
		j = (in_quote(cmd, i));
		if (i < j)
			i = j;
		if (cmd[i] == '<' || cmd[i] == '>')
			return (get_fd(list, pipex, i, cmd));
		i++;
	}
	return (0);
}

int	get_fd_redir(t_separate *list, t_data *pipex)
{
	int		check_val;

	while (1)
	{
		check_val = check_fd(list, pipex);
		if (check_val == -1)
			return (0);
		else if (check_val == 0)
			break ;
	}
	return (1);
}
