/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/04 15:22:08 by jcalon            #+#    #+#             */
/*   Updated: 2022/07/06 18:53:07 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <errno.h>
# include <stdbool.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <signal.h>
# include <linux/limits.h>

typedef struct s_pipe
{
	char			*str;
	struct s_pipe	*next;
}						t_pipe;

typedef struct s_separate
{
	char				*str;
	t_pipe				*pipe;
	struct s_separate	*next;
}						t_separate;

void	exec(t_separate *list);

void	parsing(char *cmd_line, t_separate *list);
int		syntax_error(char *str, char c);
size_t	in_quote(char *str, size_t i);

void	builtin_echo(char *options);
void	builtin_cd(char	*path);
void	builtin_pwd(void);
void	builtin_export(void);
void	builtin_unset(void);
void	builtin_env(void);
void	builtin_exit(void);

void	free_array(char	**array);
int		error_msg(char *str, int i, char c);

#endif