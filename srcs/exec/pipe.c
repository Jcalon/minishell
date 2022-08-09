/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/08 11:27:15 by jcalon            #+#    #+#             */
/*   Updated: 2022/08/09 14:43:21 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_data	data_init(t_separate *sep)
{
	t_data	pipex;

	pipex.cmds = ft_lstsize(sep->pipe);
	pipex.heredoc = 0;
	pipex.fdin = 0;
	pipex.fdout = 1;
	pipex.actual = sep->pipe;
	pipex.bouts = ft_calloc(2 * (pipex.cmds - 1), sizeof(int));
	if (pipex.bouts == NULL)
		ft_error(sep, &pipex, errmsg("PIPE ERROR", "", ""));
	pipex.pids = ft_calloc(pipex.cmds, sizeof(pid_t));
	if (pipex.pids == NULL)
		ft_error(sep, &pipex, errmsg("PID ERROR", strerror(errno), ""));
	return (pipex);
}

static void	do_children(t_data *pipex, t_separate *list, size_t i)
{
	pipex->fdin = 0;
	pipex->fdout = 1;
	if (!get_fd_redir(list, pipex))
		ft_error(list, pipex, g_return_code);
	do_var_env(list, pipex);
	if (list->str[0] == '\0')
		ft_error(list, pipex, EXIT_FAILURE);
	pipex->cmd = ft_split_minishell(list->pipe->str, " \n\t");
	clear_quote(list, pipex);
	children(pipex, i, list);
}

static void	init_children(t_data *pipex, t_separate *list, int i)
{
	if (list->fdin > 0)
		close(list->fdin);
	if (list->fdout > 1)
		close(list->fdout);
	list->fdin = -1;
	list->fdout = -1;
	list->heredoc = 0;
	pipex->pids[i] = fork();
	if (pipex->pids[i] == -1)
		ft_error(list, pipex, errmsg("Fork", ": ", strerror(errno)));
	else if (pipex->pids[i] == 0)
		do_children(pipex, list, i);
}

static int	ft_pipex(t_data *pipex, t_separate *list)
{
	int	i;
	int	exit_status;

	i = 0;
	while (i < pipex->cmds - 1)
	{
		if (pipe(pipex->bouts + 2 * i) == -1)
			ft_error(list, pipex, errmsg("PIPE CREATION ERROR", "", ""));
		i++;
	}
	i = 0;
	while (i < pipex->cmds)
	{
		init_children(pipex, list, i);
		i++;
		list->pipe = list->pipe->next;
		pipex->actual = list->pipe;
		if (list->heredoc == 1)
			unlink(".heredoc.tmp");
	}
	exit_status = parent(pipex, i);
	return (exit_status);
}

void	exec_pipe(t_separate *list)
{
	t_data	pipex;
	t_pipe	*tmp;

	tmp = list->pipe;
	pipex = data_init(list);
	g_return_code = ft_pipex(&pipex, list);
	list->pipe = tmp;
	free(pipex.bouts);
	free(pipex.pids);
}
