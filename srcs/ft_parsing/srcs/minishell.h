
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

// tokens.c
t_token	*create_token(t_cat type, char *content);
t_list	*tokenize_input(const char *input);

// tokens_helpers.c
int		file_handling(const char *input, int *i, t_list **tokens);
int		op_handling(const char *input, int *i, t_list **tokens);
int		quote_handling(const char *input, int *i, t_list **tokens, int *expect_command);
int		word_handling(const char *input, int *i, t_list **tokens,
			int *expect_command);
int		pipe_handling(const char *input, int *i, t_list **tokens);
int		token_handling(const char *input, int *i, t_list **tokens,
			int *expect_command);

// tokens_helpers2.c
void	input_handling(char *input, t_list *tokens);
void	exit_handling(char *input);
int		syntax_err_handling(char *value, t_list **tokens);
int		command_err_handling(char *value, t_list **tokens);

// utils.c
int		validate_input(const char *input);
void	free_tokens(t_list *tokens);

#endif