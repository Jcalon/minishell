/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/08 11:27:15 by jcalon            #+#    #+#             */
/*   Updated: 2022/08/04 21:40:06 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	ft_lstsize(t_pipe *pipe)
{
	t_pipe	*list;
	size_t	i;

	list = pipe;
	i = 0;
	while (list)
	{
		i++;
		list = list->next;
	}
	return (i);
}

static t_data	data_init(t_separate *sep)
{
	t_data	pipex;

	pipex.cmds = ft_lstsize(sep->pipe);
	pipex.heredoc = 0;
	pipex.fdin = 0;
	pipex.fdout = 1;
	pipex.bouts = ft_calloc(2 * (pipex.cmds - 1), sizeof(int));
	pipex.actual = sep->pipe;
	if (pipex.bouts == NULL)
		ft_error(&pipex, errmsg("PIPE ERROR", "", ""));
	pipex.pids = ft_calloc(pipex.cmds, sizeof(pid_t));
	if (pipex.pids == NULL)
		ft_error(&pipex, errmsg("PID ERROR", strerror(errno), ""));
	return (pipex);
}

static void	init_children(t_data *pipex, t_separate *list, int i)
{
	if (list->fdin > 0)
		close(list->fdin);
	if (list->fdout > 1)
		close(list->fdout);
	list->fdin = 0;
	list->fdout = 1;
	list->heredoc = 0;
	do_var_env(list);
	pipex->pids[i] = fork();
	if (pipex->pids[i] == -1)
		ft_error(pipex, errmsg("Fork", ": ", strerror(errno)));
	else if (pipex->pids[i] == 0)
	{
		pipex->fdin = 0;
		pipex->fdout = 1;
		if (!get_fd_redir(list, pipex))
			exit(EXIT_FAILURE);
		if (list->str[0] == '\0')
			exit(EXIT_FAILURE);
		pipex->cmd = ft_split_minishell(list->pipe->str, " \n\t");
		if (ft_strcmp(pipex->cmd[0], "echo"))
			clear_quote(list, pipex);
		children(pipex, i, list);
		ft_free_array(pipex->cmd);
	}
}

static int	ft_pipex(t_data *pipex, t_separate *list)
{
	int	i;
	int	exit_status;

	i = 0;
	while (i < pipex->cmds - 1)
	{
		if (pipe(pipex->bouts + 2 * i) == -1)
			ft_error(pipex, errmsg("PIPE CREATION ERROR", "", ""));
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

	pipex = data_init(list);
	g_global.return_code = ft_pipex(&pipex, list);
	
}