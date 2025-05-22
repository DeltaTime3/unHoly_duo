/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afilipe- <afilipe-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 09:41:52 by afilipe-          #+#    #+#             */
/*   Updated: 2025/05/22 16:08:29 by afilipe-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include "../libft/libft.h"
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <limits.h>

# define	MAX_PATH 4096

# define	STDERR 2
# define	E_TARG "Too may arguments for cd command.\n"
# define	E_CD	"Failed to change directory.\n"
# define	E_HOME "Directory path can not be NULL.\n"
# define	E_PERMIT "User does not have permition to access this directory.\n"
# define	E_ARGS "Too may arguments.\n"
# define	E_NOTNBR "The argument is not a number.\n"
# define	E_MSH "Invadid option.\n"
# define	E_ENV "Environment error.\n"
# define	E_IMPUT "Invalid input.\n"

typedef enum e_token_type
{
	TOKEN_CMD,
	TOKEN_ARG,
	TOKEN_PIPE,
	TOKEN_FILE,
	TOKEN_RDIR_IN,
	TOKEN_RDIR_OUT,
} t_token_type;

typedef struct s_shell
{
	t_token_type	*type;
	char			**env_var;
	char			*prev_dir;
	char			*curr_dir;
	char			*value;
	char			*pwd;
	int				return_code;
	int				exit_code;
	struct s_shell	*next;
	int	r_code;
}	t_shell;


typedef struct s_token
{
	char			*value;
	char			*token;
	char			**args;
	struct s_token	*next;
} t_token;
 
typedef struct s_env
{
	char			*key;
	char			*value;
	int				flag;
	struct s_env	*next;
}	t_env;

int	print_error(char *msg);
void	ft_putstr_fd(char *str, int fd);

//utils

int	ft_atoll(char *str, int *error);

//env funtions

t_env	*init_env(char **envp);
void	free_env_node(t_env *node);
void	free_env(t_env *head);
t_env	*create_env_node(char *str);
void	env_remove(t_env **head, char *key);
void	update_env(t_env **head, char *key, char *value, int append);
t_env *create_node_from_key(char *key, char *value);
char	*appen_env_value(char *old_val, char *add_val);
void	add_new_node(t_env **head, t_env *tail, char *key, char *value);
void	update_env_value(t_env *node, char *value, int append);

//cd

void	ft_cd(t_token *token, t_shell *type);
void	change_dir(t_token *token, char *new_dir, t_shell *type);
void	ft_cd_2(t_shell *type, char *new_dir);
char	*get_cd_target(t_token *token, t_shell *type, int *is_cd_minus);
int		dir_val(t_token *token, t_shell *type, char **new_dir);
int		check_dir(char *new_dir);
void	cd_env_pwd(t_shell *type);
void	cd_env(t_shell *type);
//test
t_token *make_token_list(const char *cmd, const char *arg);
void free_token_list(t_token *token);
t_shell *make_shell(void);
void free_shell(t_shell *sh);
int main(void);

#endif