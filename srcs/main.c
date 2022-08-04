/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/04 17:06:26 by jcalon            #+#    #+#             */
/*   Updated: 2022/08/04 21:25:04 by jcalon           ###   ########.fr       */
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

	tmp = ft_strdup("\033[32;1mminishell@\033[0m\e[1;34m");
	str = getcwd(NULL, 0);
	len = ft_strlen(str);
	count_slash = 0;
	while (--len && count_slash < 3)
		if (str[len] == '/')
			count_slash++;
	ft_join_more(&tmp, str + len + 2);
	ft_join_more(&tmp, "$>\e[0m ");
	free(str);
	return (tmp);
}

int	main(int argc, char *argv[], char *envp[])
{
	char		*buffer;
	char		*prompt;
	t_separate	list;

	(void)argc;
	(void)argv;
	g_global.return_code = 0;
	g_global.child_pid = 0;
	get_env(envp);
	while (1)
	{
		signal(SIGINT, handler);
		signal(SIGQUIT, SIG_IGN);
		prompt = ft_prompt();
		buffer = readline(prompt);
		if (!buffer)
			ft_exit();
		if (buffer[0] && (buffer[0] < 7 || buffer[0] > 13))
			add_history(buffer);
		if (!syntax_error(buffer, '|') && !syntax_error(buffer, ';') && buffer[0] != '\n')
		{
			parsing(buffer, &list);
			exec(&list);
		}
		free(buffer);
		free(prompt);
	}
	return (0);
}
