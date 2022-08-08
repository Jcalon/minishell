/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/08 12:41:21 by jcalon            #+#    #+#             */
/*   Updated: 2022/08/08 16:02:57 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	errmsg(char *str1, char *str2, char *str3)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(str1, 2);
	ft_putstr_fd(str2, 2);
	if (str3 == NULL)
		ft_putstr_fd("\n", 2);
	else
		ft_putendl_fd(str3, 2);
	return (1);
}

int	cmderr(char *str1, char *str2, char *str3)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(str1, 2);
	ft_putstr_fd(str2, 2);
	if (str3 == NULL)
		ft_putstr_fd("\n", 2);
	else
		ft_putendl_fd(str3, 2);
	g_return_code = 127;
	return (127);
}

void	ft_quit(t_separate *list)
{
	ft_free_array(list->begin->env);
	free_stuff(list->begin->next);
	close_std();
	exit(g_return_code);
}

void	ft_error(t_separate *list, t_data *pipex, int erno)
{
	close_files(pipex);
	free(pipex->bouts);
	free(pipex->pids);
	if (pipex->cmd)
		ft_free_array(pipex->cmd);
	ft_free_array(list->begin->env);
	free_stuff(list->begin->next);
	close_std();
	exit(erno);
}

void	close_files(t_data *pipex)
{
	int	j;

	j = 0;
	while (j < (pipex->cmds - 1) * 2)
	{
		if (pipex->bouts[j] != -1)
			close(pipex->bouts[j]);
		j++;
	}
	if (pipex->heredoc == 1)
		unlink(".heredoc.tmp");
}
