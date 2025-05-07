
#ifndef MINISHELL_H
# define MINISHELL_H

# define MAX_TOKENS 100
# define MAX_TOKEN_LENGTH 256

# include "libft.h"
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>

// STRUCTS

typedef enum s_cat
{
	COMMAND, // ls, echo, etc.
	ARGUMENT, // -l, file.txt, etc.
	OPERATOR, // |, <, >, >>
	DELIMETER, // spaces
	QUOTE, // ''' and '"'
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

// tokens_helpers.c
int		op_handling(const char *input, size_t *i, t_list **tokens);
int		quote_handling(const char *input, size_t *i, t_list **tokens);
int 	word_handling(const char *input, size_t *i, t_list **tokens,
    		int *expect_command);
int		token_handling(const char *input, size_t *i, t_list **tokens,
        	int *expect_command);

// tokens_helpers2.c
int		syntax_err_handling(char unexpected_char);
int		syntax_err_handling_nl(char unexpected_char);

#endif