/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-d <ppaula-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 13:59:12 by ppaula-d          #+#    #+#             */
/*   Updated: 2025/07/31 23:41:09 by ppaula-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>
# include <termios.h>

# define MAX_PATH 4096
# define BUFFER_SIZE 1024

// ERROR MESSAGES
# define UNMATCHED_QUOTES "minishell: syntax error, unmatched quotes\n"
# define UNCLOSED_QUOTES "minishell: syntax error, unclosed quotes\n"
# define LOGICAL_OPERATORS "minishell: syntax error, logical operators \
not supported according to project's subject\n"
# define UNEXPECTED_TOKEN "minishell: syntax error near unexpected token\n"
# define COMMAND_NOT_FOUND "minishell: command not found\n"
# define OPEN_PIPE "minishell: error, open pipes not supported\n"
# define STDERR 2
# define E_TARG "Too may arguments for cd command.\n"
# define E_CD "Failed to change directory.\n"
# define E_HOME "cd: HOME not set.\n"
# define E_PERMIT "User does not have permition to access this directory.\n"
# define E_ARGS " too many arguments\n"
# define E_NOTNBR "numeric argument required\n"
# define E_MSH "Invadid option.\n"
# define E_ENV "Environment error.\n"
# define E_INPUT "Invalid input.\n"
# define E_INVALID_ID "not a valid identifier"

// STRUCTS

typedef struct s_env	t_env;
typedef struct s_token	t_token;
typedef struct s_shell	t_shell;

extern int				g_global_sig;

typedef struct s_quote_handler_args
{
	const char	*input;
	int			*i;
	t_token		**tokens;
	int			*expect_command;
}	t_quote_handler_args;

typedef struct s_token_handler_args
{
	int			cond;
	int			(*handler)(const char*, int*, t_token**);
	const char	*input;
	int			*i;
	t_token		**tokens;
	int			*expect_command;
	int			expect_val;
}	t_token_handler_args;

typedef struct s_token_args
{
	const char	*quoted_content;
	const char	*unquoted_suffix;
	char		quote;
	t_token		**tokens;
	int			*expect_command;
}	t_token_args;

typedef struct s_token_create_args
{
	char		*quoted_content;
	char		*unquoted_suffix;
	char		quote;
	t_token		**tokens;
	int			*expect_command;
}	t_token_create_args;

typedef enum s_cat
{
	COMMAND,
	ARGUMENT,
	PIPE,
	REDIRECT,
	OPERATOR,
	HERE_DOC,
	DELIMETER,
	QUOTE,
	FLAG,
	FILES,
	OTHER,
	COMMENT
}	t_cat;

typedef enum e_token_type
{
	TOKEN_CMD,
	TOKEN_ARG,
	TOKEN_PIPE,
	TOKEN_FILE,
	TOKEN_RDIR_IN,
	TOKEN_RDIR_OUT,
}	t_token_type;

typedef struct s_shell
{
	t_token_type	*type;
	t_env			*head;
	t_env			*tail;
	char			*prev_dir;
	char			*curr_dir;
	char			*value;
	char			*pwd;
	t_token			*token;
	int				return_code;
	int				exit_code;
	int				heredoc_fd;
	struct s_shell	*next;
	int				r_code;
	t_token			*head_tokens;
}	t_shell;

typedef struct s_token
{
	char			*value;
	char			*token;
	char			*command;
	char			**args;
	struct s_token	*next;
	t_cat			type;
	char			*content;
	int				expand_heredoc;
	int				quote_type; // 0 = unquoted, 1 = single, 2 = double
	int				was_expanded;
	int				in_single_quotes;
	int				heredoc_fd;
}	t_token;

typedef struct s_env
{
	char			*key;
	char			*value;
	int				flag;
	struct s_env	*next;
}	t_env;

typedef struct s_exp_state
{
	char			*result;
	int				i;
	int				start;
	char			in_quote;
	t_shell			*shell;
}	t_exp_state;

typedef struct s_pipe_data
{
	int				pipe_count;
	int				pipe_fd[2];
	int				in_fd;
	pid_t			pid;
	t_token			*current;
	int				i;
	pid_t			last_pid;
	t_shell			*shell;
	int				saved_fds[2];
	t_token			*tokens;
}	t_pipe_data;

// PARSING PROTOTYPES

// expansions.c
void		append_str(char **dest, const char *src);
void		adjust_command_after_expansion(t_token *tokens);
char		*expand_env_var(const char *input, t_shell *shell);
char		*expand_exit_status(const char *input, t_shell *shell);
int			skip_quotes(const char *input, int i, char *result, int *j);
char		*remove_quotes(const char *input);
void		expand_tokens(t_token *token, t_shell *shell);
char		*expand_token_value(char *value, t_shell *shell);
char		*exp_vars_dbl_quotes(char *value, t_shell *shell);
void		handle_expansion(const char *value, int *i, char **result,
				t_shell *shell);
void		promote_next_command_if_expanded_empty(t_token *current);
char		*expand_variables(char *value, t_shell *shell);
char		*expand_single_quoted_value(char *value, t_shell *shell);
char		*process_quotes(char *input, t_shell *shell);
char		*process_quoted_segment(const char *input, int *i, char quote_type,
				t_shell *shell);
char		*process_unquoted_segment(const char *input, int *i,
				t_shell *shell);
char		*join_and_free(char *s1, char *s2);
void		handle_exit_code_expansion(char **result, t_shell *shell, int *i);
char		*handle_quoted(const char *input, int *i, t_shell *shell,
				char **result);
void		handle_single_quotes(const char *value, int *i, char **result);
void		handle_double_quotes(const char *value, int *i, char **result,
				t_shell *shell);
int			handle_env_var(const char *value, int i, char **result,
				t_shell *shell);
int			handle_dbl_quote_var(const char *value, int i, char **result,
				t_shell *shell);

// here_doc.c
int			write_heredoc_to_pipe(const char *content);
int			process_heredocs(t_token **tokens, t_shell *shell);
char		*read_heredoc_input(const char *delimiter, int expand,
				t_shell *shell);
int			join_and_check(char **content, const char *to_add);
int			read_heredoc_loop(char **content, const char *delimiter, int expand,
				t_shell *shell);
int			handle_heredoc_interrupt(char *line, char **content);
int			is_dollar_expansion(const char *value);
t_token		*find_command_start(t_token *tokens);
char		*prompt_and_read_line(void);
int			is_interrupt_or_delimiter(char *line, const char *delimiter);
char		*maybe_expand_line(char *line, int expand, t_shell *shell);
int			append_line_to_content(char **content, char *line);

// refractors.c
int			handle_quote_helper(t_quote_handler_args *args);
int			operator_type(const char *input, int *i, t_cat *type);
int			special_tokens_handling(const char *input, int *i, t_token **tokens,
				int *expect_command);
int			handle_pipe_case(const char *input, int *i, t_token **tokens,
				int *expect_command);
int			handle_heredoc_case(const char *input, int *i, t_token **tokens,
				int *expect_command);
int			handle_redirect_case(const char *input, int *i, t_token **tokens);
int			handle_token_helper(t_token_handler_args *args);
int			token_helper(const char *input, int *i, t_token **tokens,
				int *expect_command);
void		skip_special_chars(const char *input, int *i);
t_cat		determine_token_type(const char *value, int *expect_command);

// tokens.c
t_token		*create_token(t_cat type, char *value);
t_token		*tokenize_input(const char *input);
void		prep_cmd_args(t_token *head);
int			init_tokenize_vars(const char *input, t_token ***tokens,
				int *i, int *expect_command);
int			tokenize_loop(const char *input, int *i, t_token **tokens,
				int *expect_command);
int			count_cmd_args(t_token *current);
void		fill_cmd_args(t_token *current, int arg_count);

// tokens_helpers.c
char		*parse_token_part(const char *input, int *i);
int			file_handling(const char *input, int *i, t_token **tokens);
int			op_handling(const char *input, int *i, t_token **tokens);
void		scan_word_with_quotes(const char *input, int *i);
int			word_handling(const char *input, int *i, t_token **tokens,
				int *expect_command);
int			pipe_handling(const char *input, int *i, t_token **tokens);
char		*parse_heredoc_delimiter(const char *input, int *i, int *expand);
int			heredoc_handling(const char *input, int *i, t_token **tokens);
int			token_handling(const char *input, int *i, t_token **tokens,
				int *expect_command);
int			dup2_and_close(int fd, int target_fd, int *other_fd);
int			handle_pipe_error(char *heredoc_content, int fd_out,
				t_shell *shell);
int			handle_heredoc_redirect(t_token *temp, int *fd_in, int fd_out,
				t_shell *shell);
int			open_redirect_file(const char *filename, int *fd, int flags,
				t_shell *shell);
int			handle_redirect_file(const char *filename, int *fd, int flags,
				t_shell *shell);
int			handle_redirection(t_token *temp, int *fd, int flags,
				t_shell *shell);
int			close_and_return(int *fd, int ret);
int			process_redirect_token(t_token *temp, int *fd_in, int *fd_out,
				t_shell *shell);
int			process_redirects_loop(t_token *tokens, int *fd_in, int *fd_out,
				t_shell *shell);
int			redirect_handling(t_token *tokens, t_shell *shell);

// tokens_helpers2.c
void		input_handling(char *input, t_token *tokens);
void		process_input(char *input);
void		exit_handling(char *input);
void		add_token(t_token **head, t_token *new_token);

// utils.c
int			check_unmatched_quotes(const char *input);
int			check_logical_operators(const char *input);
int			check_unexpected_tokens(const char *input);
int			validate_input(const char *input);
int			handle_quote_error(t_token **tokens);
int			count_pipes(t_token *tokens);
int			handle_pipes(t_token *tokens, t_shell *shell);

// utils2.c
int			quote_handling(const char *input, int *i, t_token **tokens,
				int *expect_command);
bool		open_pipe(const char *input, int i);
void		free_tokens(t_token *tokens);
void		free_args(char **args);
int			check_token_sequence(const char *input);

int			get_quoted_content(const char *input, int *i, char quote,
				char **quoted_content);
int			get_unquoted_suffix(const char *input, int *i,
				char **unquoted_suffix);
size_t		get_unquoted_len(const char *input, int i);
int			create_and_add_token_struct(t_token_create_args *args);
int			init_quote_args(const char *input, int *i, char **quoted_content,
				char **unquoted_suffix);

void		child_process_exit(t_shell *shell, t_token *tokens, int exit_code);
void		child_process_logic(t_pipe_data *data);
void		parent_process_logic(t_pipe_data *data);
int			execute_pipe_command(t_pipe_data *data);
int			wait_for_children(pid_t last_pid);
void		set_exit_code(t_shell *shell, int last_status);
void		restore_and_close_stdio(int saved_fds[2]);
void		init_pipe_data(t_pipe_data *data, t_token *tokens, t_shell *shell);
t_token		*find_next_cmd_after_pipe(t_token *current);
int			execute_all_pipes(t_pipe_data *data, int *last_status, int *err);

void		init_shell_struct(t_shell *shell, char **envp);
void		handle_input(t_shell *shell, char *input);
int			handle_eof(char *input);
void		main_loop(t_shell *shell);
void		cleanup_shell(t_shell *shell);
int			check_inv_red(const char *input);

// BUILT-INS PROTOTYPES
int			print_error(char *msg);
void		ft_putstr_fd(char *str, int fd);

//enviroment funtions

t_env		*init_env(char **envp);
void		free_env_node(t_env *node);
void		free_env(t_env *head);
t_env		*create_env_node(char *str);
void		env_remove(t_env **head, char *key);
void		update_env(t_env **head, char *key, char *value, int append);
t_env		*create_node_from_key(char *key, char *value);
char		*append_env_value_man(char *old_val, char *add_val);
void		add_new_node(t_env **head, t_env *tail, char *key, char *value);
void		update_env_value(t_env *node, char *value, int append);
void		clean_all_resources(t_shell *shell);
void		clean_command_resources(t_shell *shell);

//cd

void		ft_cd(t_token *token, t_shell *type);
void		change_dir(char *new_dir, t_shell *type);
void		ft_cd_2(t_shell *type, char *new_dir);
char		*get_cd_target(t_token *token, t_shell *type, int *is_cd_minus);
int			dir_val(t_token *token, t_shell *type, char **new_dir);
int			check_dir(char *new_dir);
void		cd_env_pwd(t_shell *type);		
void		cd_env(t_shell *type);
int			ct_nodes(t_token *token);
char		*get_env_value(t_env *head, const char *key);
void		add_old_pwd_to_env(t_shell *type);
void		cd_change_dir(char *new, t_shell *type, int is_cd_minus);

//export
int			ft_export(t_shell *type, char **args);
int			validate_args(char *args);
int			val_fst_char(const char *args);
int			val_var_name(const char *args);
int			val_empt_operat(const char *args);
void		sort_env(t_env *head);
void		swap_env(t_env *arg1, t_env *arg2);
int			process_export(t_shell *type, char *args);
void		add_env_node(t_env **head, const char *key, char *value, int flag);
void		parse_exp_args(const char *args, char **key, char **value,
				int *flag);
t_env		*find_env_node(t_env *head, char *key);
void		update_env_value_bi(t_env *env, char *value, int flag);
void		print_env(t_shell *type);
char		*extract_key(const char *agrs, int len);
void		append_env_value(t_env *env, char *value);
void		print_export_token(t_env *head);
char		*expand_token_arg_to_value(char *value, t_shell *shell);
char		*expand_command_arg(const char *imput, t_shell *shell);
int			export_while_args(t_shell *type, char **args);
//unset
int			ft_unset(t_shell *shell, t_token *args);
int			validate_unset_args(char *args);

//exit
int			ft_exit(t_token **token, t_shell *type);
int			ft_exit2(t_shell *type, t_token *tokens, int nbr_args, char *arg);
int			ft_is_nbr(char *str);
void		ft_kill(t_shell *type, t_token *tokens, int e_code);
int			token_counter(t_token *token);
int			exit_args(t_token **token, t_shell *type, int nbr_args, char *arg);

//pwd
int			ft_pwd(t_shell *type);
int			pwd_ut(char cwd[MAX_PATH], t_shell *type);

//echo
int			ft_echo(t_token *token);
int			n_flag_validator(const char *arg);

//utils
long long	ft_atoll(char *str, int *error);
int			handle_digits(char *str, unsigned long long *res, int sign,
				int *error);

//env
void		ft_env(t_shell *type, t_token *command);
void		env_back(t_env **lst, t_env *new);
t_env		*env_lstnew(char *key, char *value, int flag);
t_env		*init_shell_env(char **enviroment);
char		**env_list_to_array(t_env *head);
void		free_env(t_env *head);
void		free_env_node(t_env *node);
void		add_new_node(t_env **head, t_env *tail, char *key, char *value);
void		update_env_value(t_env *node, char *value, int append);
t_env		*init_env(char **envp);
t_env		*create_env_node(char *str);
void		env_remove(t_env **head, char *key);
void		update_env(t_env **head, char *key, char *value, int append);
t_env		*create_node_from_key(char *key, char *value);
char		*append_env_value_man(char *old_val, char *add_val);
t_env		*create_new_node(char *env_str, t_env **head);
t_env		*create_env_with_val(char *env_str, char *equal, t_env **head);
t_env		*create_env_without_val(char *env_str, t_env **head);
void		free_not_null(void *ptr);
int			count_env(t_env *head);
void		fill_env_array(char **array, t_env *head);
void		clean_env(t_shell *shell);
void		clean_directories(t_shell *shell);
void		clean_token_type_value(t_shell *shell);
void		clean_command_resources(t_shell *shell);
void		free_and_null(void **ptr);

//executer
void		choose_b_in(t_token *token, t_shell *shell);
int			is_builtin(t_token *token);
void		restore_fds(int saved_stdout, int saved_stdin);
t_token		*find_valid_cmd(t_token *cmd);
int			handle_special_cmds(t_shell *shell, t_token *cmd, int out, int in);
int			handle_empty_cmd(t_shell *shell, int saved_stdout,
				int saved_stdin);
int			handle_dot_cmd(t_shell *shell, int saved_stdout, int saved_stdin);
int			handle_dotdot_cmd(t_shell *shell, int saved_stdout,
				int saved_stdin);
int			handle_pipes_n_restore(t_token *value, t_shell *shell, int out,
				int in);
int			handle_red_n_restore(t_token *value, t_shell *shell, int out,
				int in);
void		exec_cmd_or_b_in(t_shell *shell, t_token *cmd);
int			ft_execute(t_shell *shell, t_token *value);
void		handle_exec_error(int err, t_token *token, t_shell *shell);
int			print_cmd_not_found(t_shell *shell, t_token *token);
int			handle_exe2_err(t_shell *shell, t_token *token, char *full_path);
int			execute2(t_shell *shell, t_token *token);
char		*handle_cmd_slash(char *cmd);
char		*handle_no_path(char *cmd);
char		*get_cmd_path(char *cmd, t_env *env);
char		*check_path_dir(char *pth_cpy, char *cmd);
char		*get_next_path(char **path_temp);
char		*build_cmb_path(char *dir, char *cmd);
void		pid_zero(char *full_path, char **env_array, t_token *token,
				t_shell *shell);
int			pid_neg(char *full_path, char **env_array);
int			pid_else(char *full_path, char **env_array, t_shell *shell,
				pid_t pid);
void		free_env_array(char **env);
int			is_sp_expantion(t_token *token);
char		*fallback(char *cmd);
int			is_valid_exit_arg(char *arg);

//signals
void		ft_signals(void);
void		handle_sig_int(int sig);
void		handle_sigint2(int sig);
void		handle_sig_heredoc(int sig);
void		signal_process(t_shell *shell);
void		clean_exit(char *input);
char		*read_line(void);
void		kill_exit(t_shell *shell, int exit_code);
void		ft_rl_cleanup_after_signal(void);

int			append_exp_var(const char *input, int i, char **result,
				t_shell *shell);
int			handle_command_substitution(const char *input, int i,
				char **result);
int			handle_env_var2(const char *input, int i, char **result,
				t_shell *shell);
int			append_char(const char *input, int i, char **result);
int			handle_heredoc_token(t_token *token, t_shell *shell);
#endif