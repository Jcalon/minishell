/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/05 13:23:22 by jcalon            #+#    #+#             */
/*   Updated: 2022/08/07 20:03:49 by jcalon           ###   ########.fr       */
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

static void	exec_forked(t_separate *list)
{
	char	*save;

	signal(SIGQUIT, handler);
	if (is_builtin(list->cmds[0]) == false)
	{
		save = strdup(list->cmds[0]);
		get_absolute_path(list, list->cmds);
		if (list->cmds[0] == NULL)
		{
			g_return_code = cmderr("command not found", ": ", save);
			ft_free_array(list->begin->env);
			free_stuff(list->begin->next);
			free(save);
			exit(127);
		}
		else
			exec_cmd(list, false);
		free(save);
	}
	ft_free_array(list->begin->env);
	free_stuff(list->begin->next);
	exit(g_return_code);
}

static int	parse_token(t_separate *list)
{
	char	*str;

	if (!get_fd_redir(list, NULL))
		return (0);
	str = ft_strdup(list->str);
	do_var_env(list);
	if (list->str[0] == '\0')
	{
		free(str);
		return (0);
	}
	list->cmds = ft_split_minishell(list->str, " \n\t");
	if (ft_strcmp(list->cmds[0], "echo"))
		clear_quote(list, NULL);
	else
	{
		ft_free_array(list->cmds);
		list->cmds = ft_split_minishell(str, " \n\t");
	}
	free(str);
	if (list->cmds[0] == NULL)
		g_return_code = cmderr("command not found", ": null", NULL);
	return (1);
}

static void	exec_no_pipe(t_separate *list)
{
	int		status;
	pid_t	pid;

	status = 0;
	if (!parse_token(list))
		return ;
	if (is_builtin(list->cmds[0]) == true)
		exec_cmd(list, true);
	else
	{
		pid = fork();
		if (pid == -1)
			perror("fork");
		else if (pid > 0)
		{
			signal(SIGINT, handle_process);
			signal(SIGQUIT, handle_process);
			waitpid(pid, &status, 0);
			if (WIFEXITED(status))
				g_return_code = WEXITSTATUS(status);
			kill(pid, SIGTERM);
		}
		else
			exec_forked(list);
	}
}

void	exec(t_separate *list)
{
	t_separate	*begin;

	list = list->next;
	begin = list;
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
	free_stuff(begin);
}
