/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/04 17:06:26 by jcalon            #+#    #+#             */
/*   Updated: 2022/07/04 17:37:38 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main()
{
	char	*buffer;
	size_t	buf_size = 2048;
	char	**cmd;
	
	buffer = (char *)calloc(sizeof(char), buf_size);
	if (buffer == NULL)
	{
		perror("Malloc failure");
		return (EXIT_FAILURE);
	}
	write(1, "minishell$> ", 12);
	while (get_next_line(STDIN_FILENO))
	{
		cmd = split(buffer, " \n\t");
		if (cmd[0] == NULL)
			printf("Command not found\n");
		else if (is_builtin(cmd[0]) == false)
		{
			get_absolute_path(cmd);
			exec_cmd(cmd);
		}
		else
			exec_builtin
		write(1, "minishell$> ", 12);
	}
	free (buffer);
	return (0);
}