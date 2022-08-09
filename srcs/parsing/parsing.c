/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/05 18:31:55 by jcalon            #+#    #+#             */
/*   Updated: 2022/08/09 14:50:52 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_pipe	*add_pipe(char	*cmds)
{
	t_pipe	*new;

	new = malloc(sizeof(t_pipe));
	if (!new)
		return (NULL);
	new->next = NULL;
	new->begin = NULL;
	new->str = ft_strdup(cmds);
	return (new);
}

static t_pipe	*make_pipe(t_separate *sep)
{
	t_pipe	*pipe;
	t_pipe	*tmp;
	char	**cmds;
	size_t	i;

	i = 1;
	cmds = ft_split_minishell(sep->str, "|");
	pipe = add_pipe(cmds[0]);
	tmp = pipe;
	while (cmds[i])
	{
		tmp->next = add_pipe(cmds[i]);
		if (!tmp->next || !tmp->next->str)
			ft_error(sep, NULL, errmsg("Unexpected malloc error", "", ""));
		tmp = tmp->next;
		tmp->begin = pipe;
		i++;
	}
	ft_free_array(cmds);
	return (pipe);
}

static size_t	find_pipe(char *str, char c)
{
	size_t		i;

	i = 0;
	while (str[i])
	{
		i = in_quote(str, i);
		if (str[i] && str[i] == c)
			return (i);
		i++;
	}
	return (0);
}

/* On initialise et on check si y a des pipes
dans ce cas on cree une liste delimitee par | dans le maillon */

static t_separate	*add_list(char	*sep)
{
	t_separate	*new;

	new = malloc(sizeof(t_separate));
	if (!new)
		return (NULL);
	new->env = NULL;
	new->next = NULL;
	new->str = ft_strdup(sep);
	new->cmds = NULL;
	new->pipe = NULL;
	new->in = NULL;
	new->out = NULL;
	new->fdin = -1;
	new->fdout = -1;
	new->heredoc = 0;
	if (find_pipe(new->str, '|'))
		new->pipe = make_pipe(new);
	else
		new->pipe = NULL;
	return (new);
}

/* On va ajouter un maillon a la liste pour chaque ; */

void	parsing(char *cmd_line, t_separate *list)
{
	char		**sep;
	size_t		i;
	t_separate	*tmp;

	i = 0;
	tmp = list;
	sep = ft_split_minishell(cmd_line, ";");
	while (sep[i])
	{
		tmp->next = add_list(sep[i]);
		if (!tmp->next || !tmp->next->str)
			ft_error(list, NULL, errmsg("Unexpected malloc error", "", ""));
		tmp = tmp->next;
		tmp->begin = list;
		i++;
	}
	ft_free_array(sep);
}
