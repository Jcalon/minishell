/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/06 15:37:48 by jcalon            #+#    #+#             */
/*   Updated: 2022/07/07 17:10:16 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	in_quote(char *str, size_t i)
{
	char	c;

	while (str[i] && (str[i] == '\'' || str[i] == '"'))
	{
		c = str[i];
		i++;
		while (str[i] && str[i] != c)
		{
			if (str[i] && str[i] == '\\')
				i++;
			if (str[i])
				i++;
		}
		if (!str[i])
			break ;
		else
			i++;
	}
	return (i);
}

static int	syntax_error_redir(char *str, char c)
{
	size_t		i;
	size_t		j;

	i = 0;
	while (str[i])
	{
		j = 0;
		if ((i = (in_quote(str, i))) == ft_strlen(str))
			break ;
		while (str[i] && (str[i] == c || str[i] == ' '))
		{
			if (str[i] == c)
				j++;
			i++;
			if (j == 3)
				return (error_msg(str, i + 1, c));
			if (j > 3)
				return (error_msg(str, i, c));
		}
		if (!str[i])
			break ;
		i++;
	}
	return (0);
}

static int	syntax_error_newline(char *str, size_t i)
{
	if (str[i] == '>' || str[i] == '<')
		return (error_msg(str, i, 'n'));
	while (str[i] && (str[i] == ' ' || str[i] == '<' || str[i] == '>'))
	{
		if ((i = (in_quote(str, i))) == ft_strlen(str))
			break ;
		if (str[i] == '>' || str[i] == '<')
			return (error_msg(str, i, 'n'));
		i--;
	}
	return (0);
}

static int	syntax_error_last(char *str, size_t i, char c)
{
	if (i == 0)
		return (-1);
	i--;
	if (str[i] && str[i] == '|')
		return (error_msg(str, i, c));
	if (syntax_error_redir(str, '>') == -1 || syntax_error_redir(str, '<') == -1
		|| syntax_error_newline(str, i) == -1)
		return (-1);
	return (0);
}

int	syntax_error(char *str, char c)
{
	size_t	i;

	i = 0;
	if (!str)
		return (0);
	if (str[0] == c)
		return (error_msg(str, 0, c));
	while (str[i] && (str[i] == ' ' || str[i] == '>' || str[i] == '<'))
		i++;
	if (str[i] == c)
		return (error_msg(str, i, c));
	while (str[i])
	{
		if ((i = (in_quote(str, i))) == ft_strlen(str))
			break ;
		if (str[i] && str[i] == c)
		{
			while (str[++i] && (str[i] == ' ' || str[i] == '>' || str[i] == '<'))
				i++;
			if (str[i] == c)
				return (error_msg(str, i, c));
			if (!str[i])
				break ;
		}
		i++;	
	}
	return (syntax_error_last(str, i, c));
}