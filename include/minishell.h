/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afilipe- <afilipe-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 09:41:52 by afilipe-          #+#    #+#             */
/*   Updated: 2025/05/14 15:45:53 by afilipe-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include "./libft/libft.h"
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <limits.h>

# define	STDERR 2
# define	E_TARG "Too may arguments for cd command.\n"
# define	E_CD	"Failed to change directory.\n"
# define	E_HOME "Directory path can not be NULL.\n"
# define	E_PERMIT "User does not have permition to access this directory.\n"
# define	E_ARGS "Too may arguments.\n"
# define	E_NOTNBR "The argument is not a number.\n"

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
	int				return_code;
	int				exit_code;
	struct s_shell	*next;
	int	r_code;
}	t_shell;


typedef struct s_token
{
	char			*value;
	struct s_token	*next;
} t_token;
 

int	print_error(char *msg);
void	ft_putstr_fd(char *str, int fd);

//utils

int	ft_atoll(char *str, int *error);

#endif