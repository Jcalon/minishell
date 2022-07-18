/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/04 17:06:26 by jcalon            #+#    #+#             */
/*   Updated: 2022/07/18 18:53:07 by jcalon           ###   ########.fr       */
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

int	main(int argc, char *argv[], char *envp[])
{
	char		*buffer;
	char		*prompt;
	t_separate	list;

	(void)argc;
	(void)argv;
	list.next = NULL;
	list.pipe = NULL;
	list.str = NULL;
	list.in = NULL;
	list.out = NULL;
	list.fdin = -1;
	list.fdout = -1;
	g_global.return_code = 0;
	get_env(envp);
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
		if (!syntax_error(buffer, '|') && !syntax_error(buffer, ';') && buffer[0] != '\n')
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

// int main()
// {
// 	char	**test;
	
// 	test = ft_split_minishell("export a='test$test'", ";");
// 	// clear_quote(test);
// 	for (int i = 0; test[i]; i++)
// 		ft_putendl_fd(test[i], 1);
// }
