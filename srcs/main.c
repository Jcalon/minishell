/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/04 17:06:26 by jcalon            #+#    #+#             */
/*   Updated: 2022/07/06 18:53:53 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	prompt(void)
{
	char	*tmp;
	size_t	len;
	size_t	count_slash;

	ft_putstr_fd("\033[32;1m", 1);
	ft_putstr_fd("minishell", 1);
	ft_putstr_fd("@\033[0m:", 1);
	tmp = getcwd(NULL, 0);
	len = ft_strlen(tmp);
	count_slash = 0;
	while (--len && count_slash < 3)
		if (tmp[len] == '/')
			count_slash++;
	ft_putstr_fd("\e[1;34m", 1);
	while (tmp[++len])
		ft_putchar_fd(tmp[len], 1);
	ft_putstr_fd("$\e[0m ", 1);
	free(tmp);
}

int	main(void)
{
	char		*buffer;
	t_separate	list;

	list.next = NULL;
	list.pipe = NULL;
	list.str = NULL;
	prompt();
	buffer = get_next_line(STDIN_FILENO);
	while (buffer)
	{
		if (syntax_error(buffer, '|') != -1 && syntax_error(buffer, ';') != -1)
		{
			parsing(buffer, &list);
			exec(&list);
		}
		prompt();
		free(buffer);
		buffer = get_next_line(STDIN_FILENO);
	}
	free (buffer);
	return (0);
}
