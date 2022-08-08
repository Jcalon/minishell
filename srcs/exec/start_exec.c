/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/05 13:23:22 by jcalon            #+#    #+#             */
/*   Updated: 2022/08/08 18:32:42 by jcalon           ###   ########.fr       */
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
			list->cmds[0] = save;
			ft_quit(list);
		}
		else
		{
			free(save);
			exec_cmd(list, false);
		}
	}
	ft_quit(list);
}

static int	parse_token(t_separate *list)
{
	if (!get_fd_redir(list, NULL))
		return (0);
	do_var_env(list);
	if (list->str[0] == '\0')
		return (0);
	list->cmds = ft_split_minishell(list->str, " \n\t");
	if (!list->cmds)
		return (0);
	clear_quote(list, NULL);
	if (list->cmds[0] == NULL)
	{
		g_return_code = cmderr("command not found", ": null", NULL);
		return (0);
	}
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
