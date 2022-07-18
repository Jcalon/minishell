/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/08 11:27:15 by jcalon            #+#    #+#             */
/*   Updated: 2022/07/18 18:17:10 by jcalon           ###   ########.fr       */
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

void	get_in_out_files(t_data *pipex, char *fdin, char *fdout)
{
	// if (pipex->here_doc == 1)
	// {
	// 	get_here_doc(pipex, argv);
	// 	pipex->fdin = open(".heredoc.tmp", O_RDONLY);
	// 	if (pipex->fdin == -1)
	// 		ft_error(pipex, errmsg("ERROR HERE_DOC :", "", strerror(errno)));
	// 	pipex->fdout = open(argv[ac - 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
	// 	if (pipex->fdout == -1)
	// 		ft_error(pipex, errmsg(strerror(errno), ": ", argv[ac - 1]));
	// }
	// else
	{
		pipex->fdin = open(fdin, O_RDONLY);
		if (pipex->fdin == -1)
			errmsg(strerror(errno), ": ", fdin);
		pipex->fdout = open(fdout, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (pipex->fdout == -1)
			errmsg(strerror(errno), ": ", fdout);
	}
}

static t_data	data_init(t_separate *sep)
{
	t_data	pipex;

	pipex.cmds = ft_lstsize(sep->pipe);
	// if (!ft_strncmp("here_doc", argv[1], 9))
	// 	pipex.here_doc = 1;
	// else
	pipex.here_doc = 0;
	// else
	// 	get_in_out_files(&pipex, sep->in, sep->out);
	pipex.fdin = 0;
	pipex.fdout = 1;
	pipex.bouts = ft_calloc(2 * (pipex.cmds - 1), sizeof(int));
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
	pipex->fdin = 0;
	pipex->fdout = 1;
	do_var_env(&list->pipe->str);
	if (!get_fd_redir(&list->pipe->str, list, pipex))
		return ;
	printf("after fd %d\n", pipex->fdin);
	printf("after fd %d\n", pipex->fdout);
	if (list->in)
		clear_quote(list->in);
	pipex->cmd = ft_split_minishell(list->pipe->str, " \n\t");
	if (ft_strcmp(pipex->cmd[0], "echo"))
		clear_quote(pipex->cmd);
	pipex->pids[i] = fork();
	if (pipex->pids[i] == -1)
		ft_error(pipex, errmsg("Fork", ": ", strerror(errno)));
	if (pipex->pids[i] == 0)
		children(pipex, i);
	ft_free_array(pipex->cmd);
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