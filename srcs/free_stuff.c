/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_stuff.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/04 14:04:52 by jcalon            #+#    #+#             */
/*   Updated: 2022/08/05 15:18:57 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_content(t_separate *list)
{
	if (list->str)
		free(list->str);
	if (list->cmds)
		ft_free_array(list->cmds);
	if (list->in)
		free(list->in);
	if (list->out)
		free(list->out);
}

void	free_stuff(t_separate *list)
{
	t_separate	*tmp;
	t_pipe		*tmp2;

	while (list)
	{
		tmp = list->next;
		if (list->pipe != NULL)
		{
			while (list->pipe)
			{
				tmp2 = list->pipe->next;
				free(list->pipe->str);
				free(list->pipe);
				list->pipe = tmp2;
			}
		}
		free_content(list);
		free(list);
		list = tmp;
	}
	free(list);
}
