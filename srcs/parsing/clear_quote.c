/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clear_quote.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/11 17:54:19 by jcalon            #+#    #+#             */
/*   Updated: 2022/07/11 19:52:18 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_quote(char *str)
{
	int		count;
	size_t	i;
	size_t	k;

	count = 0;
	i = 0;
	// while (str[i] != '\0')
	// {
		while (str[i])
		{
			k = (in_quote((char *)str, i));
			if (k == ft_strlen(str))
			{
				count += 2;
				i = k;
				break;
			}
			else if (i == k)
				i++;
			else if (i < k)
			{
				count += 2;
				i = k;
			}
		}
	// }
	return (count);
}

static char	*cut_quote(char	*cmd)
{
	size_t	i;
	size_t	j;
	size_t	k;
	char	*dequoted;

	dequoted = malloc(sizeof(char) * (ft_strlen(cmd) - count_quote(cmd) + 1));
	i = 0;
	k = 0;
	while (cmd[i] != '\0')
	{
		j = in_quote(cmd, i);
		if (j > i)
		{
			i++;
			while (i < (j - 1) && cmd[i] != '\0')
				dequoted[k++] = cmd[i++];
		}
		else	
			dequoted[k++] = cmd[i++];
	}
	dequoted[k] = '\0';
	free(cmd);
	return(dequoted);
}

void	clear_quote(char **cmd)
{
	size_t	i;

	i = 0;
	while (cmd[i])
	{
		if (ft_strchr(cmd[i], '\'') || ft_strchr(cmd[i], '\"'))
			cmd[i] = cut_quote(cmd[i]);
		i++;
	}
}