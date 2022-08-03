/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/15 15:17:18 by jcalon            #+#    #+#             */
/*   Updated: 2022/08/03 20:02:21 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_istoken(int c)
{
	if (c != '<' && c != '>' && c != '|' && c != ';')
		return (0);
	return (1);
}

static int	get_fdin(size_t i, t_separate *list, t_data *pipex)
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
	if (list->in != NULL)
	{
		if (list->fdin != -1)
			close(list->fdin);
		free(list->in);
	}
	list->heredoc = 0;
	list->in = malloc(sizeof(char) * len);
	ft_strlcpy(list->in, cmd + save + whitespace + 1, len);
	list->fdin = open(list->in, O_RDONLY);
	if (list->fdin == -1)
	{
		g_global.return_code = errmsg(list->in, ": ", strerror(errno));
		return (-1);
	}
	if (pipex && list->fdin != 0)
		pipex->fdin = list->fdin;
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
		if (j == ft_strlen(cmd))
		{
			i = j;
			break;
		}
		else if (i < j)
			i = j;
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
		if (i == j)
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


// FONCTION POUR TRUNC LES REDIRS

// les trunc de cmdline et les mettre dans sep->fdin et sep->fdout

// mettre les redir dans in et check heredoc

// mettre redir dans out 