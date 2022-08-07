/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/06 15:37:48 by jcalon            #+#    #+#             */
/*   Updated: 2022/08/07 10:54:53 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	syntax_error_newline(char *str, size_t i)
{
	if (str[i] == '>' || str[i] == '<')
		return (error_msg(str, i, 'n'));
	while (str[i] && (str[i] == ' ' || str[i] == '<' || str[i] == '>'))
	{
		i = (in_quote(str, i));
		if (str[i] == '>' || str[i] == '<')
			return (error_msg(str, i, 'n'));
		i--;
	}
	return (0);
}

static int	syntax_error_last(char *str, size_t i, char c)
{
	if (i == 0)
		return (1);
	i--;
	if (str[i] && str[i] == '|')
		return (error_msg(str, i, c));
	if (syntax_error_redir(str, '>') || syntax_error_redir(str, '<')
		|| syntax_error_newline(str, i) || syntax_error_quote(str))
	{
		g_return_code = 2;
		return (1);
	}
	return (0);
}

static size_t	utils_syntax_error(size_t i, char *str, char c)
{
	while (str[i])
	{
		i = (in_quote(str, i));
		if (str[i] && str[i] == c)
		{
			while (str[++i] && (str[i] == ' ' || str[i] == '>'
					|| str[i] == '<'))
				i++;
			if (str[i] == c)
				return (error_msg(str, i, c));
			if (!str[i])
				break ;
		}
		i++;
	}
	return (i);
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
	i = utils_syntax_error(i, str, c);
	return (syntax_error_last(str, i, c));
}
