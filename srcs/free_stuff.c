/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_stuff.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/04 14:04:52 by jcalon            #+#    #+#             */
/*   Updated: 2022/08/07 17:16:26 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_content(t_separate *list)
{
	if (list->str)
		free(list->str);
	if (list->cmds)
		ft_free_array(list->cmds);
	if (list->in)
		free(list->in);
	if (list->out)
		free(list->out);
}

void	free_stuff(t_separate *list)
{
	t_separate	*tmp;
	t_pipe		*tmp2;

	while (list)
	{
		tmp = list->next;
		if (list->pipe != NULL)
		{
			while (list->pipe)
			{
				tmp2 = list->pipe->next;
				free(list->pipe->str);
				free(list->pipe);
				list->pipe = tmp2;
			}
		}
		free_content(list);
		free(list);
		list = tmp;
	}
	free(list);
}

static int	ft_error_token(char *msg, char c, int i, char *str)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(msg, 2);
	if (c == 'n')
		ft_putstr_fd("newline\'\n", 2);
	else if (str[i + 1] == c || (i > 0 && str[i - 1] == c))
	{
		ft_putchar_fd(c, 2);
		ft_putchar_fd(c, 2);
		ft_putchar_fd('\'', 2);
		ft_putchar_fd('\n', 2);
	}
	else
	{
		ft_putchar_fd(c, 2);
		ft_putchar_fd('\'', 2);
		ft_putchar_fd('\n', 2);
	}
	return (-1);
}

int	error_msg(char *str, int i, char c)
{
	ft_error_token("syntax error near unexpected token `", c, i, str);
	return (2);
}

void	ft_exit(void)
{
	g_return_code = 1;
	printf("exit\n");
	exit(g_return_code);
}
