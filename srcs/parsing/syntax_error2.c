/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/05 16:37:28 by jcalon            #+#    #+#             */
/*   Updated: 2022/08/07 11:10:06 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	more_syntax_error_quote(size_t i, char *str)
{
	char	c;
	bool	closed;

	closed = true;
	if (str[i] && (str[i] == '\'' || str[i] == '\"'))
	{
		c = str[i];
		closed = false;
		i++;
		if (str[i] && str[i] == c)
			closed = true;
		else
		{
			while (str[i] && str[i] != c)
				i++;
			if (str[i] && str[i] == c)
				closed = true;
		}
	}
	if (closed == false)
	{
		errmsg("syntax error: ", "quote doesn't guard", NULL);
		return (ft_strlen(str));
	}
	return (i);
}

int	syntax_error_quote(char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		i = more_syntax_error_quote(i, str);
		if (str[i])
			i++;
	}
	return (0);
}

int	syntax_error_redir(char *str, char c)
{
	size_t		i;
	size_t		j;

	i = 0;
	while (str[i])
	{
		i = (in_quote(str, i));
		j = 0;
		while (str[i] && str[i] == c)
		{
			if (str[i] == c)
				j++;
			i++;
			if (j == 3)
				return (error_msg(str, i + 1, c));
		}
		while (str[i] && ft_iswhitespace(str[i]))
			i++;
		if (str[i] && (str[i] == '<' || str[i] == '>') && j >= 1)
			return (error_msg(str, i, c));
		if (str[i] && str[i] != c)
			i++;
	}
	return (0);
}
