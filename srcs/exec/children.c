/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   children.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/08 12:45:06 by jcalon            #+#    #+#             */
/*   Updated: 2022/07/11 19:55:24 by jcalon           ###   ########.fr       */
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

static void	exec_child(t_data *pipex, t_pipe *pipe)
{
	pipex->cmd = ft_split_minishell(pipe->str, " \n\t");
	if (ft_strcmp(pipex->cmd[0], "echo"))
		clear_quote(pipex->cmd);
	if (!pipex->cmd)
		ft_error(pipex, errmsg("Unexpected error", "", ""));
	if (is_builtin(pipex->cmd[0]) == true)
	{
		exec_builtin(pipex->cmd);
		exit(1);
	}
	if (access(pipex->cmd[0], F_OK | X_OK) == 0)
		pipex->cmdpath = ft_strdup(pipex->cmd[0]);
	else
		pipex->cmdpath = get_absolute_path(pipex->cmd);
	if (pipex->cmdpath == NULL)
	{
		if (access(pipex->cmd[0], F_OK) == 0
			&& (access(pipex->cmd[0], X_OK) != 0))
			ft_error(pipex, errmsg(strerror(errno), ": ", pipex->cmd[0]));
		ft_error(pipex, cmderr("command not found", ": ", pipex->cmd[0]));
	}
	if (execve(pipex->cmdpath, pipex->cmd, NULL) == -1)
		ft_error(pipex, errmsg(pipex->cmd[0], ": ", strerror(errno)));
	// niel(pipex->cmd);
	// free(pipex->cmdpath);
}

void	children(t_data *pipex, t_pipe *pipe, int i)
{
	if (i == 0)
	{
		if (dup2(pipex->fdin, STDIN_FILENO) == -1)
			ft_error(pipex, 1);
		if (dup2(pipex->bouts[1], STDOUT_FILENO) == -1)
			ft_error(pipex, 1);
	}
	else if (i == pipex->cmds - 1)
	{
		if (dup2(pipex->bouts[i * 2 - 2], STDIN_FILENO) == -1)
			ft_error(pipex, 1);
		if (dup2(pipex->fdout, STDOUT_FILENO) == -1)
			ft_error(pipex, 1);
	}
	else
	{
		if (dup2(pipex->bouts[2 * i - 2], STDIN_FILENO) == -1)
			ft_error(pipex, 1);
		if (dup2(pipex->bouts[2 * i + 1], STDOUT_FILENO) == -1)
			ft_error(pipex, 1);
	}
	close_files(pipex);
	exec_child(pipex, pipe);
}