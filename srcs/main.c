/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/04 17:06:26 by jcalon            #+#    #+#             */
/*   Updated: 2022/08/07 17:16:56 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_return_code;

/* Prompt est recup par readline */

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

static void	lets_go(t_separate *list, char *buffer)
{
	parsing(buffer, list);
	exec(list);
}

/* SIGQUIT = CTRL-\ SIG_IGN pour l ignorer et SIGINT = CTRL-C */

static void	ft_signal(void)
{
	signal(SIGINT, handler);
	signal(SIGQUIT, SIG_IGN);
}

/* Dans le main on recup env et on fait une boucle infinie avec readline, 
a chaque entree on check la syntax et si c est bon lets go on parse et exec */

int	main(int argc, char *argv[], char *envp[])
{
	char		*buffer;
	char		*prompt;
	t_separate	list;

	(void)argc;
	(void)argv;
	g_return_code = 0;
	get_env(&list, envp);
	while (1)
	{
		ft_signal();
		prompt = ft_prompt();
		buffer = readline(prompt);
		free(prompt);
		if (!buffer)
			ft_exit();
		if (buffer[0] && (buffer[0] < 7 || buffer[0] > 13))
			add_history(buffer);
		if (!syntax_error(buffer, '|') && !syntax_error(buffer, ';')
			&& buffer[0] != '\n')
			lets_go(&list, buffer);
		free(buffer);
	}
	rl_clear_history();
	return (0);
}
