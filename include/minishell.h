
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

# define	MAX_PATH 4096

extern int	global_sig;

// ERROR MESSAGES
# define 	UNMATCHED_QUOTES "minishell: syntax error, unmatched quotes\n"
# define 	UNCLOSED_QUOTES "minishell: syntax error, unclosed quotes\n"
# define 	LOGICAL_OPERATORS "minishell: syntax error, logical operators \
not supported according to project's subject\n"
# define 	UNEXPECTED_TOKEN "minishell: syntax error near unexpected token\n"
# define 	COMMAND_NOT_FOUND "minishell: command not found\n"
# define 	OPEN_PIPE "minishell: error, open pipes not supported\n"
# define	STDERR 2
# define	E_TARG "Too may arguments for cd command.\n"
# define	E_CD	"Failed to change directory.\n"
# define	E_HOME "cd: HOME not set.\n"
# define	E_PERMIT "User does not have permition to access this directory.\n"
# define	E_ARGS " too many arguments\n"
# define	E_NOTNBR "numeric argument required\n"
# define	E_MSH "Invadid option.\n"
# define	E_ENV "Environment error.\n"
# define	E_INPUT "Invalid input.\n"
# define    E_INVALID_ID "not a valid identifier"

// STRUCTS

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
	COMMENT,
	REDIR_OUT,
	REDIR_IN,
	REDIR_APPEND,
	FILENAME
}	t_cat;


typedef struct s_env t_env;
typedef struct s_token t_token;
typedef struct s_shell t_shell;

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
	int	r_code;
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
} t_token;
 
typedef struct s_env
{
	char			*key;
	char			*value;
	int				flag;
	struct s_env	*next;
}	t_env;

typedef struct s_exp_state
{
	char *result;
	int		i;
	int		start;
	char	in_quote;
	t_shell	*shell;
}				t_exp_state;


// PARSING PROTOTYPES

// expansions.c
void adjust_command_after_expansion(t_token *tokens);
char *expand_env_var(const char *input, t_shell *shell);
char *expand_exit_status(const char *input, t_shell *shell);
char *remove_quotes(const char *input);
void expand_tokens(t_token *token, t_shell *shell);
char *expand_token_value(char *value, t_shell *shell);
char *expand_variables_in_double_quotes(char *value, t_shell *shell);
char *expand_variables(char *value, t_shell *shell);
char *process_quotes(char *input, t_shell *shell);

// here_doc.c
char    *read_heredoc_input(const char *delimiter, int expand, t_shell *shell);

// refractors.c
int		operator_type(const char *input, int *i, t_cat *type);
int		special_tokens_handling(const char *input, int *i, t_token **tokens,
			int *expect_command);
int		token_helper(const char *input, int *i, t_token **tokens,
			int *expect_command);
void	skip_special_chars(const char *input, int *i);
t_cat	determine_token_type(const char *value, int *expect_command);

// tokens.c
t_token	*create_token(t_cat type, char *value);
t_token	*tokenize_input(const char *input);
void	prep_cmd_args(t_token *head);
//t_token *find_command_token(t_token *head);

// tokens_helpers.c
int		file_handling(const char *input, int *i, t_token **tokens);
int		op_handling(const char *input, int *i, t_token **tokens);
int		word_handling(const char *input, int *i, t_token **tokens,
			int *expect_command);
int		pipe_handling(const char *input, int *i, t_token **tokens);
int		heredoc_handling(const char *input, int *i, t_token **tokens);
int		token_handling(const char *input, int *i, t_token **tokens,
			int *expect_command);
int	redirect_handling(t_token *tokens, t_shell *shell);

// tokens_helpers2.c
//void 	print_tokens(t_token *tokens);
void	input_handling(char *input, t_token *tokens);
void	process_input(char *input);
void	exit_handling(char *input);
void	add_token(t_token **head, t_token *new_token);

// utils.c
int		check_unmatched_quotes(const char *input);
int		check_logical_operators(const char *input);
int		check_unexpected_tokens(const char *input);
int		validate_input(const char *input);
int		handle_quote_error(t_token **tokens);
int 	count_pipes(t_token *tokens);
int 	handle_pipes(t_token *tokens, t_shell *shell);
// int 	execute_command(t_token *command_token);

// utils2.c
int		quote_handling(const char *input, int *i, t_token **tokens,
			int *expect_command);
bool	open_pipe(const char *input, int i);
void	free_tokens(t_token *tokens);
void	free_args(char **args);
int		check_token_sequence(const char *input);
int		apply_lonely_redirs(t_token *tokens, t_shell *shell);


// BUILT-INS PROTOTYPES
int	print_error(char *msg);
void	ft_putstr_fd(char *str, int fd);


//enviroment funtions

t_env	*init_env(char **envp);
void	free_env_node(t_env *node);
void	free_env(t_env *head);
t_env	*create_env_node(char *str);
void	env_remove(t_env **head, char *key);
void	update_env(t_env **head, char *key, char *value, int append);
t_env 	*create_node_from_key(char *key, char *value);
char	*append_env_value_man(char *old_val, char *add_val);
void	add_new_node(t_env **head, t_env *tail, char *key, char *value);
void	update_env_value(t_env *node, char *value, int append);
void	clean_all_resources(t_shell *shell);
void	clean_command_resources(t_shell *shell);

//cd

void	ft_cd(t_token *token, t_shell *type);
void	change_dir(char *new_dir, t_shell *type);
void	ft_cd_2(t_shell *type, char *new_dir);
char	*get_cd_target(t_token *token, t_shell *type, int *is_cd_minus);
int		dir_val(t_token *token, t_shell *type, char **new_dir);
int		check_dir(char *new_dir);
void	cd_env_pwd(t_shell *type);	
void	cd_env(t_shell *type);
int		ct_nodes(t_token *token);
char	*get_env_value(t_env *head, const char *key);
void 	add_old_pwd_to_env(t_shell *type);
void	cd_change_dir(char *new, t_shell *type, int is_cd_minus);
//test


//export
int		ft_export(t_shell *type, char **args);
int		validate_args(char *args);
int		val_fst_char(const char *args);
int		val_var_name(const char *args);
int		val_empt_operat(const char *args);
void	sort_env(t_env *head);
void	swap_env(t_env *arg1, t_env *arg2);
int		process_export(t_shell *type, char *args);
void	add_env_node(t_env **head, const char *key, char *value, int flag);
void 	parse_exp_args(const char *args, char **key, char **value, int *flag);
t_env 	*find_env_node(t_env *head, char *key);
void 	update_env_value_bi(t_env *env, char *value, int flag);
void	print_env(t_shell *type);
char 	*extract_key(const char *agrs, int len);
void 	append_env_value(t_env *env, char *value);
void	print_export_token(t_env *head);
char    *expand_token_arg_to_value(char *value, t_shell *shell);
char	*expand_command_arg(const char *imput, t_shell *shell);
int		export_while_args(t_shell *type, char **args);
char	*fallback(char *cmd);

//unset
int 	ft_unset(t_shell *shell, t_token *args);
int 	validate_unset_args(char *args);

//exit
int		ft_exit(t_token **token, t_shell *type);
int		ft_exit2(t_shell *type, t_token *tokens, int nbr_args, char *arg);
int		ft_is_nbr(char *str);
void	ft_kill(t_shell *type, t_token *tokens, int e_code);
int		token_counter(t_token *token);
int		exit_args(t_token **token, t_shell *type, int nbr_args, char *arg);
int		is_valid_exit_arg(char *arg);

//pwd
int		ft_pwd(t_shell *type);
int		pwd_ut(char cwd[MAX_PATH], t_shell *type);

//echo
int		ft_echo(t_token *token);
int		n_flag_validator(const char *arg);

//utils
long long	ft_atoll(char *str, int *error);
int			handle_digits(char *str, unsigned long long *res, int sign, int *error);

//env
void 	ft_env(t_shell *type, t_token *command);
void 	env_back(t_env **lst, t_env *new);
t_env	*env_lstnew(char *key, char *value, int flag);
t_env 	*init_shell_env(char **enviroment);
char	**env_list_to_array(t_env *head);
void	free_env(t_env *head);
void	free_env_node(t_env *node);
void	add_new_node(t_env **head, t_env *tail, char *key, char *value);
void	update_env_value(t_env *node, char *value, int append);
t_env	*init_env(char **envp);
t_env	*create_env_node(char *str);
void	env_remove(t_env **head, char *key);
void	update_env(t_env **head, char *key, char *value, int append);
t_env	 *create_node_from_key(char *key, char *value);
char	*append_env_value_man(char *old_val, char *add_val);
t_env	*create_new_node(char *env_str, t_env **head);
t_env	*create_env_with_val(char *env_str, char *equal, t_env **head);
t_env	*create_env_without_val(char *env_str, t_env **head);
void 	free_not_null(void *ptr);
int		count_env(t_env *head);
void	fill_env_array(char **array, t_env *head);
void	clean_env(t_shell *shell);
void	clean_directories(t_shell *shell);
void	clean_token_type_value(t_shell *shell);
void	clean_command_resources(t_shell *shell);
void	free_and_null(void **ptr);

//executer
void	choose_b_in(t_token *token, t_shell *shell);
int		is_builtin(t_token *token);
int		ft_execute(t_shell *shell, t_token *value);
int		execute2(t_shell *shell, t_token *token);
char	*get_cmd_path(char *cmd, t_env *env);
char	*check_path_dir(char *pth_cpy, char *cmd);
char	*get_next_path(char **path_temp);
char	*build_cmb_path(char *dir, char *cmd);
void	pid_zero(char *full_path, char **env_array, t_token *token);
int		pid_neg(char *full_path, char **env_array);
int		pid_else(char *full_path, char **env_array, t_shell *shell, pid_t pid, int sts);
void 	free_env_array(char **env);
int		is_sp_expantion(t_token *token);

//signals
void	ft_signals(void);
void	handle_sig_int(int sig);
void	handle_sig_heredoc(int sig);
void	handle_sig_pipe(int sig);
void	signal_process(t_shell *shell);
void	clean_exit(char *input);

#endif