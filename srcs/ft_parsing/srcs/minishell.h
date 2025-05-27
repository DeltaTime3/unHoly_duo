
#ifndef MINISHELL_H
# define MINISHELL_H

# define MAX_TOKENS 100
# define MAX_TOKEN_LENGTH 256

# include "libft.h"
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>

// ERROR MESSAGES
# define UNMATCHED_QUOTES "minishell: syntax error, unmatched quotes\n"
# define LOGICAL_OPERATORS "minishell: syntax error, logical operators \
not supported according to project's subject\n"
# define UNEXPECTED_TOKEN "minishell: syntax error near unexpected token\n"
# define COMMAND_NOT_FOUND "minishell: Command not found\n"
# define OPEN_PIPE "minishell: error, open pipes not supported\n"

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
	COMMENT
}	t_cat;

typedef struct s_token
{
	t_cat	type;
	char	*content;
}	t_token;

// PROTOTYPES

// refractors.c
int		operator_type(const char *input, int *i, t_cat *type);
int		special_tokens_handling(const char *input, int *i, t_list **tokens,
			int *expect_command);
int		token_helper(const char *input, int *i, t_list **tokens,
			int *expect_command);
void	skip_special_chars(const char *input, int *i);
t_cat	determine_token_type(const char *value, int *expect_command);

// tokens.c
t_token	*create_token(t_cat type, char *content);
t_list	*tokenize_input(const char *input);

// tokens_helpers.c
int		file_handling(const char *input, int *i, t_list **tokens);
int		op_handling(const char *input, int *i, t_list **tokens);
int		word_handling(const char *input, int *i, t_list **tokens,
			int *expect_command);
int		pipe_handling(const char *input, int *i, t_list **tokens);
int		token_handling(const char *input, int *i, t_list **tokens,
			int *expect_command);

// tokens_helpers2.c
void	input_handling(char *input, t_list *tokens);
void	process_input(char *input);
void	exit_handling(char *input);
// int		syntax_err_handling(char *value, t_list **tokens);
// int		command_err_handling(char *value, t_list **tokens);

// utils.c
int		check_unmatched_quotes(const char *input);
int		check_logical_operators(const char *input);
int		check_unexpected_tokens(const char *input);
int		validate_input(const char *input);
int		handle_quote_error(t_list **tokens);
// int 	execute_command(t_token *command_token);

// utils2.c
int		quote_handling(const char *input, int *i, t_list **tokens,
			int *expect_command);
bool	open_pipe(const char *input, int i);
void	free_tokens(t_list *tokens);

#endif