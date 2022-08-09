/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   children.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/08 12:45:06 by jcalon            #+#    #+#             */
/*   Updated: 2022/08/09 15:41:13 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	parent(t_data *pipex, int i)
{
	pid_t	wpid;
	int		status;
	int		exit_status;	

	exit_status = 1;
	close_files(pipex);
	i--;
	while (i >= 0)
	{
		wpid = waitpid(pipex->pids[i], &status, 0);
		if (wpid == pipex->pids[pipex->cmds - 1])
		{
			if ((i == (pipex->cmds - 1)) && WIFEXITED(status))
				exit_status = WEXITSTATUS(status);
		}
		i--;
	}
	return (exit_status);
}

static void	exec_child(t_data *pipex, t_separate *list)
{
	char	*save;

	close_files(pipex);
	if (!pipex->cmd)
		ft_error(list, pipex, errmsg("Unexpected error", "", ""));
	if (is_builtin(pipex->cmd[0]) == true)
	{
		exec_builtin(list, pipex);
		ft_free_array(pipex->cmd);
		free(pipex->bouts);
		free(pipex->pids);
		ft_quit(list);
	}
	save = strdup(pipex->cmd[0]);
	get_absolute_path(list, pipex->cmd);
	if (pipex->cmd[0] == NULL)
	{
		pipex->cmd[0] = save;
		if (access(save, F_OK) == 0
			&& (access(save, X_OK) != 0))
			ft_error(list, pipex, errmsg(strerror(errno), ": ", save));
		ft_error(list, pipex, cmderr("command not found", ": ", save));
	}
	if (execve(pipex->cmd[0], pipex->cmd, list->begin->env) == -1)
		ft_error(list, pipex, errmsg(pipex->cmd[0], ": ", strerror(errno)));
}

static void	dup_even_more(t_separate *list, t_data *pipex, int i)
{
	if (pipex->fdin != 0)
	{
		if (dup2(pipex->fdin, STDIN_FILENO) == -1)
			ft_error(list, pipex, errmsg(strerror(errno), "", ""));
	}
	else if (dup2(pipex->bouts[2 * i - 2], STDIN_FILENO) == -1)
		ft_error(list, pipex, errmsg(strerror(errno), "", ""));
	if (pipex->fdout != 1)
	{
		if (dup2(pipex->fdout, STDOUT_FILENO) == -1)
			ft_error(list, pipex, errmsg(strerror(errno), "", ""));
	}
	else if (dup2(pipex->bouts[2 * i + 1], STDOUT_FILENO) == -1)
		ft_error(list, pipex, errmsg(strerror(errno), "", ""));
}

static void	dup_more(t_separate *list, t_data *pipex, int i)
{
	if (i == pipex->cmds - 1)
	{
		if (pipex->fdin != 0)
		{
			if (dup2(pipex->fdin, STDIN_FILENO) == -1)
				ft_error(list, pipex, errmsg(strerror(errno), "", ""));
		}
		else if (dup2(pipex->bouts[i * 2 - 2], STDIN_FILENO) == -1)
			ft_error(list, pipex, errmsg(strerror(errno), "", ""));
		if (dup2(pipex->fdout, STDOUT_FILENO) == -1)
			ft_error(list, pipex, errmsg(strerror(errno), "", ""));
	}
	else
		dup_even_more(list, pipex, i);
}

void	children(t_data *pipex, int i, t_separate *list)
{
	if (pipex->heredoc == 1)
	{
		pipex->fdin = open(".heredoc.tmp", O_RDONLY, 0644);
		if (pipex->fdin == -1)
			g_return_code = errmsg("Heredoc", ": ", strerror(errno));
	}
	if (i == 0)
	{
		if (dup2(pipex->fdin, STDIN_FILENO) == -1)
			ft_error(list, pipex, errmsg(strerror(errno), "", ""));
		if (pipex->fdout != 1)
		{
			if (dup2(pipex->fdout, STDOUT_FILENO) == -1)
				ft_error(list, pipex, errmsg(strerror(errno), "", ""));
		}
		else if (dup2(pipex->bouts[1], STDOUT_FILENO) == -1)
			ft_error(list, pipex, errmsg(strerror(errno), "", ""));
	}
	else
		dup_more(list, pipex, i);
	if (list->fdin != -1)
		close(list->fdin);
	if (list->fdout != -1)
		close(list->fdout);
	exec_child(pipex, list);
}
