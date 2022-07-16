/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/15 15:17:18 by jcalon            #+#    #+#             */
/*   Updated: 2022/07/16 18:17:54 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_istoken(int c)
{
	if (c != '<' && c != '>' && c != '|' && c != ';')
		return (0);
	return (1);
}

static void	get_fdin(char **cmds, size_t ind, size_t i, t_separate *list)
{
	size_t	len;
	size_t	j;
	size_t	k;
	size_t	save;
	char	*newline;

	len = 0;
	i++;
	while (ft_iswhitespace(cmds[ind][i]))
		i++;
	save = i;
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
	list->in = malloc(sizeof(char *) * (2));
	list->in[0] = malloc(sizeof(char) * (len));
	ft_strlcpy(list->in[0], cmds[ind] + save, len + 1);
	newline = malloc(sizeof(char) * (ft_strlen(cmds[ind] - len + 1)));
	k = 0;
	while (k < save - 1)
	{
		newline[k] = cmds[ind][k];
		k++;
	}
	ft_strcpy(cmds[ind] + save + len + 1, newline + k);
	free (cmds[ind]);
	cmds[ind] = newline;
}

static int	check_fd(char	**cmds, size_t ind, t_separate *list)
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
			get_fdin(cmds, ind, i, list);
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
	return (i);
}

void	get_fd_redir(char **cmds, t_separate *list)
{
	size_t	i;

	i = 0;
	while (cmds[i])
	{
		if (ft_strchr(cmds[i], '<'))
		{
			// while (1)
			// {
				if (!check_fd(cmds, i, list))
					break ;
			// }
		}
		i++;
	}
}


// FONCTION POUR TRUNC LES REDIRS

// les trunc de cmdline et les mettre dans sep->fdin et sep->fdout

// mettre les redir dans in et check heredoc

// mettre redir dans out 