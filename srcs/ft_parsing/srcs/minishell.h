
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
	COMMAND, // ls, echo, etc.
	ARGUMENT, // -l, file.txt, etc.
	PIPE,
	REDIRECT, // <, >, >>
	OPERATOR, // other operators
	HERE_DOC,
	DELIMETER, // spaces
	QUOTE, // ''' and '"'z
	COMMENT // #
}   t_cat;

typedef struct s_token
{
	t_cat type; // token type (COMMAND, ARGUMENT, OPERATOR, DELIMETER, QUOTE, COMMENT)
	char *content; // token value ("ls", "-l")
}   t_token;

// PROTOTYPES

// tokens.c
t_token	*create_token(t_cat type, char *content);
t_list	*tokenize_input(const char *input);

// tokens_helpers.c
int		op_handling(const char *input, size_t *i, t_list **tokens);
int		quote_handling(const char *input, size_t *i, t_list **tokens);
int 	word_handling(const char *input, size_t *i, t_list **tokens,
    		int *expect_command);
int		pipe_handling(const char *input, size_t *i, t_list **tokens);
int		token_handling(const char *input, size_t *i, t_list **tokens,
        	int *expect_command);

// tokens_helpers2.c
void    input_handling(char *input, t_list *tokens);
void 	exit_handling(char *input);
int		syntax_err_handling(char *value, t_list **tokens);

// utils.c
int		handle_op_syntax(t_token *next);
void    free_tokens(t_list *tokens);

#endif