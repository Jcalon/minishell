/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_minishell.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/02 15:22:29 by jcalon            #+#    #+#             */
/*   Updated: 2022/08/07 13:21:16 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	countword(char const *str, char *c)
{
	int		count;
	size_t	i;
	size_t	j;

	count = 0;
	i = 0;
	while (str[i] != '\0')
	{
		while (str[i] && is_c(str[i], c))
			i++;
		if (str[i])
			count++;
		while (str[i] && !is_c(str[i], c))
		{
			j = in_quote((char *)str, i);
			if (i < j)
				i = j;
			else
				i++;
		}
	}
	return (count);
}

static int	countletter(char const *s, char *c)
{
	int	i;
	int	j;

	i = 0;
	while (s[i])
	{
		if (is_c(s[i], c))
			break ;
		j = (int)in_quote((char *)s, i);
		if (i < j)
			i = j;
		else
			i++;
	}
	return (i);
}

static void	freesplit(int j, char **res)
{
	while (j >= 0)
	{
		free(res[j]);
		j--;
	}
	free(res);
}

static void	make_str(char *s, int tabi[3], char *c, char **res)
{
	size_t	j;

	j = 0;
	while (s[tabi[0]])
	{
		if (is_c(s[tabi[0]], c))
			break ;
		j = in_quote(s, tabi[0]);
		if ((int)j > tabi[0])
		{
			while (tabi[0] < (int)j && s[tabi[0]] != '\0')
				res[tabi[1]][tabi[2]++] = s[tabi[0]++];
		}
		else
			res[tabi[1]][tabi[2]++] = s[tabi[0]++];
	}
	res[tabi[1]][tabi[2]] = '\0';
}

char	**ft_split_minishell(char const *s, char *c)
{
	char	**res;
	int		tabi[3];

	tabi[0] = 0;
	tabi[1] = -1;
	res = malloc((countword(s, c) + 1) * sizeof(char *));
	if (!res)
		return (NULL);
	while (++tabi[1] < countword(s, c) && s[0])
	{
		tabi[2] = 0;
		while (is_c(s[tabi[0]], c))
			tabi[0]++;
		res[tabi[1]] = malloc((countletter(s + tabi[0], c) + 1) * sizeof(char));
		if (!res[tabi[1]])
		{
			freesplit(tabi[1], res);
			return (NULL);
		}
		make_str((char *)s, tabi, c, res);
	}
	res[tabi[1]] = 0;
	return (res);
}
