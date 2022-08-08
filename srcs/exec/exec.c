/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/07 16:50:07 by jcalon            #+#    #+#             */
/*   Updated: 2022/08/08 15:50:39 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_builtin(t_separate *list, t_data *pipex)
{
	char	**cmds;

	if (pipex)
		cmds = pipex->cmd;
	else
		cmds = list->cmds;
	if (!ft_strcmp(cmds[0], "echo"))
		builtin_echo(list, pipex);
	else if (!ft_strcmp(cmds[0], "cd"))
		builtin_cd(list, pipex);
	else if (!ft_strcmp(cmds[0], "pwd"))
		builtin_pwd(list, pipex);
	else if (!ft_strcmp(cmds[0], "export"))
		builtin_export(list, pipex);
	else if (!ft_strcmp(cmds[0], "unset"))
		builtin_unset(list, pipex);
	else if (!ft_strcmp(cmds[0], "env"))
	{	
		if (ft_array_size(cmds) == 1)
			builtin_env(false, list);
		else
			g_return_code = errmsg("env: ", "too many args", NULL);
	}
	else if (!ft_strcmp(cmds[0], "exit"))
		builtin_exit(list, pipex);
}

static void	open_fd_cmd(t_separate *list)
{
	if (list->heredoc == 1)
	{
		list->fdin = open(".heredoc.tmp", O_RDONLY, 0644);
		if (list->fdin == -1)
			g_return_code = errmsg(list->in, ": ", strerror(errno));
	}
	if (list->fdin != -1)
	{
		if (dup2(list->fdin, STDIN_FILENO) == -1)
			g_return_code = 1;
		close(list->fdin);
	}
	if (list->fdout != -1)
	{
		if (dup2(list->fdout, STDOUT_FILENO) == -1)
			g_return_code = 1;
		close(list->fdout);
	}
}

void	exec_cmd(t_separate *list, bool builtin)
{
	if (builtin == false)
	{
		open_fd_cmd(list);
		if (execve(list->cmds[0], list->cmds, list->begin->env) == -1)
		{
			g_return_code = cmderr("command not found", ": ", list->cmds[0]);
			ft_quit(list);
		}
	}
	else if (builtin == true)
		exec_builtin(list, NULL);
}

static int	make_absolute_path(char **paths, char **cmd)
{
	size_t	i;
	char	*tmp;
	char	*cmdpath;

	i = 0;
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		cmdpath = ft_strjoin(tmp, cmd[0]);
		free(tmp);
		if (access(cmdpath, X_OK) == 0)
		{
			free(cmd[0]);
			cmd[0] = cmdpath;
			return (1);
		}
		free(cmdpath);
		i++;
	}
	return (0);
}

void	get_absolute_path(t_separate *list, char **cmd)
{
	char	**paths;

	if (access(cmd[0], F_OK | X_OK) == 0)
		return ;
	paths = get_path(list);
	if (paths == NULL)
		return ;
	if (make_absolute_path(paths, cmd) == 1)
	{
		ft_free_array(paths);
		return ;
	}
	ft_free_array(paths);
	free(cmd[0]);
	cmd[0] = NULL;
}
