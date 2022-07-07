/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/06 13:57:32 by jcalon            #+#    #+#             */
/*   Updated: 2022/07/07 17:13:53 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int		ft_error_token(char *msg, char c, int i, char *str)
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

int		error_msg(char *str, int i, char c)
{
	ft_error_token("syntax error near unexpected token `", c, i, str);
	return (-1);
}

void	ft_exit(void)
{
	g_global.return_code = 1;
	printf("exit\n");
	// if (count_array(av) > 1)
	// {
	// 	if (!ft_strisdigit(av[1]))
	// 	{
	// 		ft_error2("exit", av[1], "numeric argument required", 255);
	// 		return ;
	// 	}
	// 	else if (count_array(av) != 2)
	// 	{
	// 		ft_error2("exit", av[1], "too many arguments", 1);
	// 	}
	// 	else
	// 		g_global.return_code = ft_atoi(av[1]);
	// }
	exit(g_global.return_code);
}


void	free_array(char	**array)
{
	size_t	i;

	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}
