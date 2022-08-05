/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/08 12:41:21 by jcalon            #+#    #+#             */
/*   Updated: 2022/08/05 15:19:21 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*errinfo(char *str1, char *str2)
{
	ft_putstr_fd(str1, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(str2, 2);
	return (NULL);
}

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

void	ft_error(t_data *pipex, int erno)
{
	(void)pipex;
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
