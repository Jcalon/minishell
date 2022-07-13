/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/04 15:22:08 by jcalon            #+#    #+#             */
/*   Updated: 2022/07/13 15:08:50 by jcalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <errno.h>
# include <stdbool.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <signal.h>
# include <string.h>
# include <linux/limits.h>
# include <readline/readline.h>
# include <readline/history.h>

typedef struct s_global
{
	char	**env;
	int		child_pid;
	int		return_code;
}			t_global;

extern t_global	g_global;

typedef struct s_pipe
{
	char			*str;
	struct s_pipe	*next;
}					t_pipe;

typedef struct s_separate
{
	char				*str;
	t_pipe				*pipe;
	struct s_separate	*next;
	char				*in;
	char				*out;
}						t_separate;

typedef struct s_data
{
	int		*bouts;
	char	*cmdpath;
	pid_t	*pids;
	int		fdin;
	int		fdout;
	char	**cmd;
	int		cmds;
	int		here_doc;
}				t_data;

char	*ft_prompt(void);

void	handler(int sig_num);

void	ft_add_history(char *buffer);
void	ft_load_history(void);

void	exec(t_separate *list);
void	exec_cmd(char **cmd);
void	exec_pipe(t_separate *list);
bool	is_builtin(char *cmd);
char	*get_absolute_path(char **cmd);
void	exec_builtin(char **builtin);

void	parsing(char *cmd_line, t_separate *list);
int		syntax_error(char *str, char c);
size_t	in_quote(char *str, size_t i);

int		builtin_echo(char **options);
int		builtin_cd(char	*path);
int		builtin_pwd(void);
int		builtin_export(char **cmd);
int		builtin_unset(char **cmd);
int		builtin_env(bool export);
int		builtin_exit(char *str);
int		check_double_env(char *str, size_t len);
size_t	ft_strlen_equal(const char *s);

void	free_array(char	**array);
int		error_msg(char *str, int i, char c);
void	ft_exit(void);

char	*errinfo(char *str1, char *str2);
int		errmsg(char *str1, char *str2, char *str3);
int		cmderr(char *str1, char *str2, char *str3);
void	ft_error(t_data *pipex, int err);
void	close_files(t_data *pipex);
void	children(t_data *pipex, t_pipe *pipe, int i);
int		parent(t_data *pipex, int i);

void	get_env(char **envp);
char	**get_path();
char	*ft_getenv(char *str);
char	*ft_get_var_env(char *str, size_t len);
void	do_var_env(char **cmds);
char	**ft_split_minishell(char const *s, char *c);

void	clear_quote(char **cmd);

#endif