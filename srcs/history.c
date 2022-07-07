/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/07 14:08:30 by jcalon            #+#    #+#             */
/*   Updated: 2022/07/07 16:57:19 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_history_file(void)
{
	char	*home;

	home = getenv("HOME");
	home = ft_strjoin(home, "/.minishell_history");
	return (home);
}

void	ft_load_history(void)
{
	int				fd;
	char			*buffer;
	char			*history_file;

	history_file = get_history_file();
	fd = open(history_file, O_CREAT | O_RDWR | O_APPEND, 0777);
	buffer = get_next_line(fd);
	while (buffer)
	{
		add_history(buffer);
		free(buffer);
		buffer = get_next_line(fd);
	}
	free(history_file);
	close(fd);
}

void	ft_add_history(char *buffer)
{
	int				fd;
	char			*history_file;

	history_file = get_history_file();
	fd = open(history_file, O_CREAT | O_RDWR | O_APPEND, 0777);
	ft_putstr_fd(buffer, fd);
	ft_putstr_fd("\n", fd);
	add_history(buffer);
	free(history_file);
	close(fd);
}
