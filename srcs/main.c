/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/04 17:06:26 by jcalon            #+#    #+#             */
/*   Updated: 2022/09/02 14:40:54 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_status;

/* Prompt est recup par readline */

char	*ft_prompt(void)
{
	char	*tmp;
	char	*str;
	size_t	len[2];

	tmp = ft_strdup("\033[32;1mminishell@\033[0m\e[1;34m");
	if (!tmp)
	{
		close_std();
		exit(errmsg("Unexpected malloc error", "", ""));
	}
	str = getcwd(NULL, 0);
	if (str)
	{
		len[0] = ft_strlen(str);
		len[1] = 0;
		if (len[0] > 3)
		{
			while (--len[0] && len[1] < 3)
				if (str[len[0]] == '/')
					len[1]++;
			ft_join_more(&tmp, str + len[0] + 1);
		}
	}
	ft_join_more(&tmp, "$>\e[0m ");
	return (free(str), tmp);
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

static int	test_empty(char *buffer)
{
	size_t	i;

	i = 0;
	while (buffer[i] && ft_iswhitespace(buffer[i]))
		i++;
	if (i != ft_strlen(buffer))
		return (1);
	return (0);
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
	g_status = 0;
	get_env(&list, envp);
	while (1)
	{
		ft_signal();
		prompt = ft_prompt();
		buffer = readline(prompt);
		free(prompt);
		if (!buffer)
			ft_exit(&list);
		if (buffer[0] && test_empty(buffer))
			add_history(buffer);
		if (!syntax_error(buffer, '|') && !syntax_error(buffer, ';')
			&& test_empty(buffer))
			lets_go(&list, buffer);
		free(buffer);
	}
	return (0);
}
