/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crazyd <crazyd@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/05 13:23:22 by jcalon            #+#    #+#             */
/*   Updated: 2022/07/21 09:59:04 by crazyd           ###   ########.fr       */
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

void	exec_builtin(t_separate *list)
{
	if (!ft_strcmp(list->cmds[0], "echo"))
		builtin_echo(list);
	else if (!ft_strcmp(list->cmds[0], "cd"))
		builtin_cd(list);
	else if (!ft_strcmp(list->cmds[0], "pwd"))
		builtin_pwd(list);
	else if (!ft_strcmp(list->cmds[0], "export"))
		builtin_export(list);
	else if (!ft_strcmp(list->cmds[0], "unset"))
		builtin_unset(list);
	else if (!ft_strcmp(list->cmds[0], "env"))
		if (ft_array_size(list->cmds) == 1)
			builtin_env(false, list);
		else
			g_global.return_code = errmsg("env: ", "too many args", NULL);
	else if (!ft_strcmp(list->cmds[0], "exit"))
		builtin_exit(list);
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
			perror("minishell");
	}
	else if (builtin == true)
		exec_builtin(list);
	// exit(EXIT_FAILURE);
}

char	*get_absolute_path(char **cmd)
{
	char	*cmd_absolute;
	char	*tmp;
	char	**paths;
	size_t	i;

	if (access(cmd[0], F_OK | X_OK) == 0)
		return (cmd[0]);
	i = 0;
	paths = get_path();
	if (paths == NULL)
		return (NULL);
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		cmd_absolute = ft_strjoin(tmp, cmd[0]);
		free(tmp);
		if (access(cmd_absolute, X_OK) == 0)
			return (cmd_absolute);
		free(cmd_absolute);
		i++;
	}
	return (NULL);
}

void	exec_no_pipe(t_separate *list)
{
	char	*cmdpath;
	int		status;

	status = 0;
	signal(SIGQUIT, handler);
	g_global.child_pid = fork();
	if (g_global.child_pid == -1)
		perror("fork");
	else if (g_global.child_pid > 0)
	{
		waitpid(g_global.child_pid, &status, 0);
		kill(g_global.child_pid, SIGTERM);
		do_var_env(list);
		if (!get_fd_redir(list, NULL))
			return ;
		if (list->str[0] == '\0')
			return ;
		list->cmds = ft_split_minishell(list->str, " \n\t");
		if (ft_strcmp(list->cmds[0], "echo"))
			clear_quote(list);
		if (list->cmds[0] == NULL)
			g_global.return_code = cmderr("command not found", ": null", NULL);
		else if (is_builtin(list->cmds[0]) == true)
			exec_cmd(list, true);
	}
	else
	{
		do_var_env(list);
		if (!get_fd_redir(list, NULL))
			exit(EXIT_FAILURE);
		if (list->str[0] == '\0')
			exit(EXIT_FAILURE);
		list->cmds = ft_split_minishell(list->str, " \n\t");
		if (ft_strcmp(list->cmds[0], "echo"))
			clear_quote(list);
		if (list->cmds[0] == NULL)
			g_global.return_code = cmderr("command not found", ": null", NULL);
		else if (is_builtin(list->cmds[0]) == false)
		{
			cmdpath = get_absolute_path(list->cmds);
			if (cmdpath == NULL)
			{
				g_global.return_code = cmderr("command not found", ": ", list->cmds[0]);
				exit(EXIT_FAILURE);	
			}
			else
			{
				free(list->cmds[0]);
				list->cmds[0] = cmdpath;
				exec_cmd(list, false);
			}
		}
		exit(EXIT_SUCCESS);
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
