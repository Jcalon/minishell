/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/15 15:17:18 by jcalon            #+#    #+#             */
/*   Updated: 2022/07/18 18:53:17 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_istoken(int c)
{
	if (c != '<' && c != '>' && c != '|' && c != ';')
		return (0);
	return (1);
}

static int	get_fdin(char **cmds, size_t ind, size_t i, t_separate *list, t_data *pipex)
{
	size_t	len;
	size_t	j;
	size_t	k;
	size_t	save;
	size_t	whitespace;
	char	*newline;

	len = 1;
	save = i;
	i++;
	whitespace = 1;
	while (ft_iswhitespace(cmds[ind][i++]))
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
	list->in[0] = malloc(sizeof(char) * (len + 1));
	ft_strlcpy(list->in[0], cmds[ind] + save + whitespace, len + 1);
	list->fdin = open(list->in[0], O_RDONLY);
	if (list->fdin == -1)
	{
		g_global.return_code = errmsg(list->in[0], ": ", strerror(errno));
		return (-1);
	}
	if (pipex && list->fdin != 0)
		pipex->fdin = list->fdin;
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

			// if (list->in)
			// {
			// 	list->fdin = open(list->in[0], O_RDONLY);
			// 	if (list->fdin == -1)
			// 	{
			// 		g_global.return_code = errmsg(list->in[0], ": ", "No such file or directory");
			// 		break ;
			// 	}
			// }

static int	check_fd(char	**cmds, size_t ind, t_separate *list, t_data *pipex)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (cmds[ind][i])
	{
		j = (in_quote(cmds[ind], i));
		if (j == ft_strlen(cmds[ind]))
		{
			i = j;
			break;
		}
		else if (i < j)
			i = j;
		// if (cmds[ind][i] == '<' && cmds[ind][i + 1] == '<')
		// {
		// 	get_heredoc(cmds, ind, i, list);
		// 	i++;
		// 	return (1);
		// }
		// else 
		if (cmds[ind][i] == '<')
		{
			if (get_fdin(cmds, ind, i, list, pipex) == -1)
				return (-1);
			return (1);
		}
		else if (cmds[ind][i] == '>')
		{
			if (get_fdout(cmds, ind, i, list, pipex) == -1)
				return (-1);
			return (1);
		}
		// else if (cmds[ind][i] == '>' && cmds[ind][i + 1] == '>')
		// {
		// 	get_fdout_appen(cmds, ind, i, list);
		// 	i++;
		// 	return (1);
		// }
		// else if (cmds[ind][i] == '<')
		// {
		// 	get_fdout(cmds, ind, i, list);
		// 	return (1);
		// }
		if (i == j)
			i++;
	}
	return (0);
}

int	get_fd_redir(char **cmds, t_separate *list, t_data *pipex)
{
	size_t	i;
	int		check_val;

	i = 0;
	if (ft_strchr(cmds[i], '<') || ft_strchr(cmds[i], '>'))
	{
		while (1)
		{
			check_val = check_fd(cmds, i, list, pipex);
			if (check_val == -1)
				return (0);
			else if (check_val == 0)
				break ;
		}
	}
	return (1);
}


// FONCTION POUR TRUNC LES REDIRS

// les trunc de cmdline et les mettre dans sep->fdin et sep->fdout

// mettre les redir dans in et check heredoc

// mettre redir dans out 