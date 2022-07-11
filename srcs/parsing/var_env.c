/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/11 15:24:26 by jcalon            #+#    #+#             */
/*   Updated: 2022/07/11 16:12:44 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	check_varenv(char *str)
// {
// 	size_t	i;
	
// 	i = 0;
// 	while (str[i])
// 	{
// 		if ((i = (in_quote(str, i))) == ft_strlen(str))
// 			break ;
// 		if (str[i] && str[i] == c)
// 		{
// 			while (str[++i] && (str[i] == ' ' || str[i] == '>' || str[i] == '<'))
// 				i++;
// 			if (str[i] == c)
// 				return (error_msg(str, i, c));
// 			if (!str[i])
// 				break ;
// 		}
// 		i++;	
// 	}
// }