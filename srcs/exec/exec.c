/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/05 13:23:22 by jcalon            #+#    #+#             */
/*   Updated: 2022/07/12 15:10:30 by jcalon           ###   ########.fr       */
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

void	exec_builtin(char **cmd)
{
	if (!ft_strcmp(cmd[0], "echo"))
		builtin_echo(cmd);
	else if (!ft_strcmp(cmd[0], "cd"))
		builtin_cd(cmd[1]);
	else if (!ft_strcmp(cmd[0], "pwd"))
		builtin_pwd();
	else if (!ft_strcmp(cmd[0], "export"))
		builtin_export();
	else if (!ft_strcmp(cmd[0], "unset"))
		builtin_unset();
	else if (!ft_strcmp(cmd[0], "env"))
		builtin_env();
	else if (!ft_strcmp(cmd[0], "exit"))
	{
		if (cmd[1] != NULL)
			builtin_exit(cmd[1]);
		else
		{
			ft_putendl_fd("exit", 2);
			exit(0);
		}
	}
}

void	exec_cmd(char **cmd)
{
	pid_t	pid;
	int		status;

	status = 0;
	pid = fork();
	if (pid == -1)
		perror("fork");
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		kill(pid, SIGTERM);
	}
	else
	{
		if (execve(cmd[0], cmd, NULL) == -1)
			perror("minishell");
		exit(EXIT_FAILURE);
	}
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

void	exec(t_separate *list)
{
	char	**cmd;
	char	*cmdpath;

	list = list->next;
	while (list)
	{
		if (list->pipe == NULL)
		{
			cmd = ft_split_minishell(list->str, " \n\t");
			do_var_env(cmd);
			if (ft_strcmp(cmd[0], "echo"))
				clear_quote(cmd);
			if (cmd[0] == NULL)
				ft_printf("Command not found\n");
			else if (is_builtin(cmd[0]) == false)
			{
				cmdpath = get_absolute_path(cmd);
				if (cmdpath == NULL)
					cmderr("command not found", ": ", cmd[0]);
				else
				{
					free(cmd[0]);
					cmd[0] = cmdpath;
					exec_cmd(cmd);
				}
			}
			else
				exec_builtin(cmd);
		}
		else
			exec_pipe(list);
		list = list->next;
	}
}
