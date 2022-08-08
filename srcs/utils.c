/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/06 13:57:32 by jcalon            #+#    #+#             */
/*   Updated: 2022/08/08 19:25:21 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	in_quote(char *str, size_t i)
{
	char	c;

	while (str[i] && (str[i] == '\'' || str[i] == '"'))
	{
		c = str[i];
		i++;
		while (str[i] && str[i] != c)
				i++;
		break ;
	}
	return (i);
}

int	is_c(char s, char *c)
{
	size_t	i;

	i = 0;
	while (c[i] != '\0')
	{
		if (c[i] == s)
			return (1);
		i++;
	}
	return (0);
}

int	test_fdout(t_separate *list)
{
	if (list->out)
		return (list->fdout);
	return (STDOUT_FILENO);
}

size_t	ft_lstsize(t_pipe *pipe)
{
	t_pipe	*list;
	size_t	i;

	list = pipe;
	i = 0;
	while (list)
	{
		i++;
		list = list->next;
	}
	return (i);
}

void	close_std(void)
{
	close(STDERR_FILENO);
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
}
