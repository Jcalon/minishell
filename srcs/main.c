/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/04 17:06:26 by jcalon            #+#    #+#             */
/*   Updated: 2022/07/08 17:13:46 by jcalon           ###   ########.fr       */
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

char	**get_path()
{
	char	*path;
	char	**paths;

	path = ft_strdup(getenv("PATH"));
	paths = ft_split(path, ":");
	return (paths);
}

int	main(void)
{
	char		*buffer;
	char		*prompt;
	t_separate	list;

	list.next = NULL;
	list.pipe = NULL;
	list.str = NULL;
	g_global.env = get_path();
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
		if (buffer[0] && (buffer[0] < 7 || buffer[0] > 13))
		{
			if (buffer[ft_strlen(buffer) - 1] == '\n')
				buffer[ft_strlen(buffer) - 1] = '\0';
			ft_add_history(buffer);
		}
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
