/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/14 13:51:03 by jcalon            #+#    #+#             */
/*   Updated: 2022/09/02 14:40:55 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Afficher la(les) CHANE(s) en écho sur la sortie standard.
	-n     ne pas effectuer le saut de ligne final
	sachant que -n doit être avant les CHAINES
	Si $ -> expand si n'est pas entre ' ' */

static size_t	check_n(char **cmd)
{
	size_t	i;
	size_t	j;

	i = 1;
	while (cmd[i] && cmd[i][0] == '-' && cmd[i][1] == 'n')
	{
		j = 1;
		while (cmd[i][j])
		{
			if (cmd[i][j] != 'n')
				return (i);
			j++;
		}
		i++;
	}
	return (i);
}

static size_t	print_var_env(t_separate *list, char **cmds, size_t i, size_t j)
{
	size_t	k;

	if (cmds[i][j + 1] && cmds[i][j + 1] == '?')
	{
		ft_putnbr_fd(g_status, test_fdout(list));
		j += 2;
	}
	else if (cmds[i][j + 1] && ft_isalpha(cmds[i][j + 1]))
	{
		k = 0;
		j++;
		while (ft_isalnum(cmds[i][j + k]))
			k++;
		ft_putstr_fd(ft_get_var_env(list, cmds[i] + j, k), test_fdout(list));
		j += k;
	}
	else
	{
		if (!ft_isd(cmds[i][j + 1]) && (cmds[i][j + 1] == '\"'
			&& cmds[i][j + 2] == '\0'))
			j += write(test_fdout(list), "$", 1);
		while (cmds[i][j] && (cmds[i][j + 1] == '$' || ft_isd(cmds[i][j + 1])))
			j++;
	}
	return (j);
}

static size_t	print_echo(t_separate *list, char **cmds, size_t i, size_t j)
{
	char	c;

	c = 0;
	if (cmds[i][j] == '\'' || cmds[i][j] == '\"')
	{
		c = cmds[i][j];
		j++;
		while (cmds[i][j] && cmds[i][j] == c)
		{
			c = '\0';
			if ((cmds[i][j + 1] == '\'' || cmds[i][j + 1] == '\"'))
				c = cmds[i][j + 1];
			j++;
		}
	}
	while (cmds[i][j] && cmds[i][j] != c && c != '\0')
	{
		if (cmds[i][j] && cmds[i][j] == '$' && c != '\'')
			j = print_var_env(list, cmds, i, j);
		else
			write(test_fdout(list), &cmds[i][j++], 1);
	}
	return (j);
}

static void	ft_echo(t_separate *list, char **cmds, size_t i)
{
	size_t	j;
	size_t	k;

	while (cmds[i])
	{
		j = 0;
		k = 0;
		while (cmds[i][j])
		{
			k = print_echo(list, cmds, i, j);
			if (k > j)
				j = k;
			else if (cmds[i][j] && cmds[i][j] != '$')
				j += write(test_fdout(list), &cmds[i][j], 1);
			else if (ft_isalnum(cmds[i][j + 1]) || cmds[i][j + 1] == '\0')
				j += write(test_fdout(list), &cmds[i][j], 1);
			else
				j++;
		}
		if (cmds[i + 1] != NULL)
			write(test_fdout(list), " ", 1);
		i++;
	}
}

void	builtin_echo(t_separate *list, t_data *pipex)
{
	size_t	i;
	bool	n;
	char	**cmds;

	if (pipex)
		cmds = pipex->cmd;
	else
		cmds = list->cmds;
	n = false;
	i = check_n(cmds);
	if (i > 1)
		n = true;
	ft_echo(list, cmds, i);
	if (n == false)
		write(test_fdout(list), "\n", 1);
	g_status = 0;
}
