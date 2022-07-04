/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/02 15:22:29 by jcalon            #+#    #+#             */
/*   Updated: 2022/05/26 11:46:30 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	countword(char const *str, char c)
{
	int		count;
	size_t	i;

	count = 0;
	i = 0;
	while (str[i] != '\0')
	{
		while (str[i] && str[i] == c)
			i++;
		if (str[i])
			count++;
		while (str[i] && str[i] != c)
			i++;
	}
	return (count);
}

static int	countletter(char const *s, char c)
{
	int	i;

	i = 0;
	while (s[i] && s[i] != c)
		i++;
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

char	**ft_split(char const *s, char c)
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
		while (s[tabi[0]] == c)
			tabi[0]++;
		res[tabi[1]] = malloc((countletter(s + tabi[0], c) + 1) * sizeof(char));
		if (!res[tabi[1]])
		{
			freesplit(tabi[1], res);
			return (NULL);
		}
		while (s[tabi[0]] != c && s[tabi[0]] != '\0')
			res[tabi[1]][tabi[2]++] = s[tabi[0]++];
		res[tabi[1]][tabi[2]] = '\0';
	}
	res[tabi[1]] = 0;
	return (res);
}
