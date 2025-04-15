
#ifndef MINISHELL_H
# define MINISHELL_H

# define MAX_TOKENS 100
# define MAX_TOKEN_LENGTH 256

# include "libft.h"
# include <stdio.h>
# include <stdlib.h>

// STRUCTS

typedef enum s_tokens
{
    COMMAND, // ls, echo, etc.
    ARGUMENT, // -l, file.txt, etc.
    OPERATOR, // |, <, >, >>
    DELIMETER, // spaces
    QUOTE, // ''' and '"'
    COMMENT // #
}   t_tokens;

typedef struct s_token
{
    t_tokens type; // token type (COMMAND, ARGUMENT, OPERATOR, DELIMETER, QUOTE, COMMENT)
    char *content; // token value ("ls", "-l")
}   t_token;

// PROTOTYPES

t_token *create_token(t_tokens type, char *content);


#endif