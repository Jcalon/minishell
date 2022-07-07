/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/04 17:06:26 by jcalon            #+#    #+#             */
/*   Updated: 2022/07/07 19:42:13 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_global	g_global;

char	*ft_prompt(void)
{
	char	*tmp;
	char	*str;
	size_t	len;
	size_t	count_slash;

	tmp = ft_strdup("\033[32;1mminishell@\033[0m:\e[1;34m");
	str = getcwd(NULL, 0);
	len = ft_strlen(str);
	count_slash = 0;
	while (--len && count_slash < 3)
		if (str[len] == '/')
			count_slash++;
	ft_join_more(&tmp, str + len + 2);
	ft_join_more(&tmp, "$\e[0m ");
	free(str);
	return (tmp);
}

int	main(void)
{
	char		*buffer;
	char		*prompt;
	t_separate	list;

	list.next = NULL;
	list.pipe = NULL;
	list.str = NULL;
	signal(SIGINT, handler);
	signal(SIGQUIT, handler);
	ft_load_history();
	while (1)
	{
		prompt = ft_prompt();
		buffer = readline(prompt);
			// 	if (!in)
			// ft_exit(0);
		if (!buffer)
			ft_exit();
		if (buffer[0])
			ft_add_history(buffer);
		if (syntax_error(buffer, '|') != -1 && syntax_error(buffer, ';') != -1)
		{
			parsing(buffer, &list);
			exec(&list);
		}
		free(buffer);
		free(prompt);
	}
	free (buffer);
	return (0);
}
