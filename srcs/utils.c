/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/06 13:57:32 by jcalon            #+#    #+#             */
/*   Updated: 2022/08/05 15:18:40 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
