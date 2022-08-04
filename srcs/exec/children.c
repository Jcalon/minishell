/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   children.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/08 12:45:06 by jcalon            #+#    #+#             */
/*   Updated: 2022/08/04 21:38:03 by jcalon           ###   ########.fr       */
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
	// ft_clean(pipex);
	return (exit_status);
}

static void	exec_child(t_data *pipex, t_separate *list)
{
	char	*save;
	if (!pipex->cmd)
		ft_error(pipex, errmsg("Unexpected error", "", ""));
	if (is_builtin(pipex->cmd[0]) == true)
	{
		exec_builtin(list, pipex);
		exit(g_global.return_code);
	}
	save = strdup(pipex->cmd[0]);
	get_absolute_path(pipex->cmd);
	if (pipex->cmd[0] == NULL)
	{
		if (access(pipex->cmd[0], F_OK) == 0
			&& (access(pipex->cmd[0], X_OK) != 0))
			ft_error(pipex, errmsg(strerror(errno), ": ", save));
		ft_error(pipex, cmderr("command not found", ": ", save));
	}
	if (execve(pipex->cmd[0], pipex->cmd, NULL) == -1)
		ft_error(pipex, errmsg(pipex->cmd[0], ": ", strerror(errno)));
	// niel(pipex->cmd);
	// free(pipex->cmdpath);
}

void	children(t_data *pipex, int i, t_separate *list)
{
	if (pipex->heredoc == 1)
	{
		pipex->fdin = open(".heredoc.tmp", O_RDONLY, 0644);
		if (pipex->fdin == -1)
			g_global.return_code = errmsg("Heredoc", ": ", strerror(errno));
	}
	if (i == 0)
	{
		if (dup2(pipex->fdin, STDIN_FILENO) == -1)
			ft_error(pipex, 1);
		if (pipex->fdout != 1)
		{
			if (dup2(pipex->fdout, STDOUT_FILENO) == -1)
				ft_error(pipex, 1);
		}
		else if (dup2(pipex->bouts[1], STDOUT_FILENO) == -1)
			ft_error(pipex, 1);
	}
	else if (i == pipex->cmds - 1)
	{
		if (pipex->fdin != 0)
		{
			if (dup2(pipex->fdin, STDIN_FILENO) == -1)
				ft_error(pipex, 1);
		}
		else if (dup2(pipex->bouts[i * 2 - 2], STDIN_FILENO) == -1)
			ft_error(pipex, 1);
		if (dup2(pipex->fdout, STDOUT_FILENO) == -1)
			ft_error(pipex, 1);
	}
	else
	{
		if (pipex->fdin != 0)
		{
			if (dup2(pipex->fdin, STDIN_FILENO) == -1)
				ft_error(pipex, 1);
		}
		else if (dup2(pipex->bouts[2 * i - 2], STDIN_FILENO) == -1)
			ft_error(pipex, 1);
		if (pipex->fdout != 1)
		{
			if (dup2(pipex->fdout, STDOUT_FILENO) == -1)
				ft_error(pipex, 1);
		}
		else if (dup2(pipex->bouts[2 * i + 1], STDOUT_FILENO) == -1)
			ft_error(pipex, 1);
	}
	close_files(pipex);
	exec_child(pipex, list);
}
