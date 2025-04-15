
#ifndef MINISHELL_H
# define MINISHELL_H

# define MAX_TOKENS 100
# define MAX_TOKEN_LENGTH 256

typedef enum s_tokens
{
    COMMAND,
    ARGUMENT,
    OPERATOR,
    DELIMETER,
    QUOTE,
    COMMENT
}   t_tokens;

typedef struct s_token
{
    t_tokens type; // token type
    char *value; // token value
}   t_token;

#endif