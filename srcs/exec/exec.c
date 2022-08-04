/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/05 13:23:22 by jcalon            #+#    #+#             */
/*   Updated: 2022/08/04 21:37:23 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_builtin(char *cmd)
{
	const char	*builtin[] = {"echo", \
		"cd", "pwd", "export", "unset", "env", "exit", NULL};
	size_t		i;

	i = 0;
	while (builtin[i])
	{
		if (!ft_strcmp(builtin[i], cmd))
			return (true);
		i++;
	}
	return (false);
}

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
		if (ft_array_size(cmds) == 1)
			builtin_env(false, list);
		else
			g_global.return_code = errmsg("env: ", "too many args", NULL);
	else if (!ft_strcmp(cmds[0], "exit"))
		builtin_exit(list, pipex);
}

void	exec_cmd(t_separate *list, bool builtin)
{
	if (builtin == false)
	{
		if (list->heredoc == 1)
		{
			list->fdin = open(".heredoc.tmp", O_RDONLY, 0644);
			if (list->fdin == -1)
				g_global.return_code = errmsg(list->in, ": ", strerror(errno));
		}
		if (list->fdin != -1)
		{
			if (dup2(list->fdin, STDIN_FILENO) == -1)
				g_global.return_code = 1;	
			close(list->fdin);
		}
		if (list->fdout != -1)
		{
			if (dup2(list->fdout, STDOUT_FILENO) == -1)
				g_global.return_code = 1;
			close(list->fdout);
		}
		if (execve(list->cmds[0], list->cmds, NULL) == -1)
		{
			cmderr(list->cmds[0], ": command not found", NULL);
			exit(126);
		}
	}
	else if (builtin == true)
		exec_builtin(list, NULL);
}

void	get_absolute_path(char **cmd)
{
	char	*tmp;
	char	*cmdpath;
	char	**paths;
	size_t	i;

	if (access(cmd[0], F_OK | X_OK) == 0)
		return ;
	i = 0;
	paths = get_path();
	if (paths == NULL)
		return ;
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		cmdpath = ft_strjoin(tmp, cmd[0]);
		free(tmp);
		if (access(cmdpath, X_OK) == 0)
		{
			free(cmd[0]);
			cmd[0] = cmdpath;
			return ;
		}
		free(cmdpath);
		i++;
	}
	free(cmd[0]);
	cmd[0] = NULL;
}

void	exec_no_pipe(t_separate *list)
{
	int		status;
	char	*save;

	status = 0;
	do_var_env(list);
	g_global.return_code = 0;
	if (!get_fd_redir(list, NULL))
		return ;
	if (list->str[0] == '\0')
		return ;
	list->cmds = ft_split_minishell(list->str, " \n\t");
	if (ft_strcmp(list->cmds[0], "echo"))
		clear_quote(list, NULL);
	if (list->cmds[0] == NULL)
		g_global.return_code = cmderr("command not found", ": null", NULL);
	g_global.child_pid = fork();
	if (g_global.child_pid == -1)
		perror("fork");
	else if (g_global.child_pid > 0)
	{
		signal(SIGQUIT, handler);
		waitpid(g_global.child_pid, &status, 0);
		if (WIFEXITED(status))
			g_global.return_code = WEXITSTATUS(status);
		kill(g_global.child_pid, SIGTERM);
		if (is_builtin(list->cmds[0]) == true)
			exec_cmd(list, true);
	}
	else
	{
		signal(SIGQUIT, handler);
		if (is_builtin(list->cmds[0]) == false)
		{
			save = strdup(list->cmds[0]);
			get_absolute_path(list->cmds);
			if (list->cmds[0] == NULL)
			{
				g_global.return_code = cmderr("command not found", ": ", save);
				free(save);
				exit(127);	
			}
			// else if (list->cmds[0] == NULL && save[0] == '/')
			// {
			// 	g_global.return_code = cmderr(save, ": Not a directory", NULL);
			// 	free(save);
			// 	exit(126);
			// }
			else
				exec_cmd(list, false);
			free(save);
		}
		free_stuff(list);
		exit(g_global.return_code);
	}
}

void	exec(t_separate *list)
{
	list = list->next;
	while (list)
	{
		if (list->pipe == NULL)
			exec_no_pipe(list);
		else
			exec_pipe(list);
		if (list->heredoc == 1)
				unlink(".heredoc.tmp");
		list = list->next;
	}
}
