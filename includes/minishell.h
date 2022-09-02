/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/04 15:22:08 by jcalon            #+#    #+#             */
/*   Updated: 2022/09/02 16:22:05 by jcalon           ###   ########.fr       */
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

extern int	g_status;

typedef struct s_pipe
{
	char			*str;
	struct s_pipe	*next;
	struct s_pipe	*begin;
}					t_pipe;

typedef struct s_separate
{
	char				**env;
	char				*str;
	char				**cmds;
	t_pipe				*pipe;
	struct s_separate	*next;
	struct s_separate	*begin;
	char				*in;
	char				*out;
	int					fdin;
	int					fdout;
	int					heredoc;
}						t_separate;

typedef struct s_data
{
	int				*bouts;
	char			*cmdpath;
	pid_t			*pids;
	int				fdin;
	int				fdout;
	char			**cmd;
	int				cmds;
	int				heredoc;
	struct s_pipe	*actual;
}				t_data;

char	*ft_prompt(void);

/* Pour recup l'env */

void	get_env(t_separate *list, char **envp);
char	**get_path(t_separate *list);
char	*ft_getenv(t_separate *list, char *str);
char	*ft_get_var_env(t_separate *list, char *str, size_t len);

/* Gestionnaire des signaux */

void	handler(int sig_num);
void	handle_process(int sig_num);

/* Check pbm de syntax */

int		syntax_error(char *str, char c);
int		syntax_error_quote(char *str);
int		syntax_error_redir(char *str, char c);

void	parsing(char *cmd_line, t_separate *list);

/* Gestion des redirections */

int		get_fd_redir(t_separate *list, t_data *pipex);
int		get_fdin(size_t i, t_separate *list, t_data *pipex);
int		get_fdout(size_t i, t_separate *list, t_data *pipex);
int		get_fdout_append(size_t i, t_separate *list, t_data *pipex);
int		get_heredoc(size_t i, t_separate *list, t_data *pipex);
int		ft_istoken(int c);
size_t	get_fd_name_len(char *cmd, size_t *size, size_t i);
void	remove_redir(t_separate *list, t_data *pipex, size_t *size, char *cmd);

/* Expand */

int		check_var_env(t_separate *list, t_data *pipex, size_t check);
char	*replace_by_code_var_env(char *cmd);
size_t	in_single_quote(char *str, size_t i);
int		counterr(int err);
void	do_var_env(t_separate *list, t_data *pipex);
char	*ft_test_pipe(t_separate *list, t_data *pipex);

void	clear_quote(t_separate *list, t_data *pipex);
char	*cut_quote(t_separate *list, t_data *pipex, char *cmd);

/* Execution */

void	exec(t_separate *list);
bool	is_builtin(char *cmd);
void	get_absolute_path(t_separate *list, char **cmd);
void	exec_cmd(t_separate *list, bool builtin);
void	exec_builtin(t_separate *list, t_data *pipex);
void	exec_pipe(t_separate *list);
int		parent(t_data *pipex, int i);
void	children(t_data *pipex, int i, t_separate *list);

/* Builtins */

void	builtin_echo(t_separate *list, t_data *pipex);
int		test_fdout(t_separate *list);
void	builtin_cd(t_separate *list, t_data *pipex);
void	builtin_pwd(t_separate *list);
void	builtin_export(t_separate *list, t_data *pipex);
void	*new_env_malloc(char **new_env, size_t size);
char	*concat_new(char *cmd);
size_t	num_to_export(t_separate *list, char **cmd);
int		check_double_env(t_separate *list, char *str, size_t len);
size_t	ft_strlen_equal(const char *s);
void	ft_unset_swap_env(t_separate *list, char **new_env, size_t k);
void	ft_check_dup(t_separate *list, t_data *pipex, size_t k, char **new_env);
void	builtin_unset(t_separate *list, t_data *pipex);
void	builtin_env(bool export, t_separate *list);
void	builtin_exit(t_separate *list, t_data *pipex);

/* Erreur, free et quit */

void	free_array(char	**array);
void	free_stuff(t_separate *list);
void	close_std(void);
void	close_files(t_data *pipex);
int		error_msg(char *str, int i, char c);
int		errmsg(char *str1, char *str2, char *str3);
int		cmderr(char *str1, char *str2, char *str3, int code);
void	ft_error(t_separate *list, t_data *pipex, int err);
void	ft_exit(t_separate *list);
void	ft_quit(t_separate *list);

/* Utils */

size_t	in_quote(char *str, size_t i);
size_t	ft_lstsize(t_pipe *pipe);
int		is_c(char s, char *c);
int		test_empty(char *buffer);
char	**ft_split_minishell(char const *s, char *c);

#endif