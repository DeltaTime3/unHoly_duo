#include "minishell.h"

// files
int	file_handling(const char *input, int *i, t_token **tokens)
{
	size_t	start;
	char	*value;

	while (input[*i] && ft_isspace(input[*i]))
		(*i)++;
	start = *i;
	while (input[*i] && !ft_isspace(input[*i]))
		(*i)++;
	value = ft_substr(input, start, *i - start);
	if (!value)
		return (1);
	add_token(tokens, create_token(FILES, value));
	free(value);
	return (0);
}

// operators
int	op_handling(const char *input, int *i, t_token **tokens)
{
	int	start;
	char	*value;
	t_cat	type;

	start = *i;
	value = NULL;
	if (operator_type(input, i, &type))
		return (1);
	value = ft_substr(input, start, *i - start + 1);
	if (!value || ft_strlen(value) == 0)
    {
        free(value);
        return (1);
    }
	add_token(tokens, create_token(type, value));
	free(value);
	(*i)++;
	while (input[*i] && ft_isspace(input[*i]))
		(*i)++;
	if (type == REDIRECT || type == HERE_DOC)
	{
		if (input[*i] && file_handling(input, i, tokens))
			return (1);
	}
	return (0);
}

// words
int	word_handling(const char *input, int *i, t_token **tokens,
	int *expect_command)
{
	size_t	start;
	char	*value;
	t_cat	type;

	start = *i;
	skip_special_chars(input, i);
	value = ft_substr(input, start, *i - start);
	if (!value || ft_strlen(value) == 0)
    {
        free(value);
        return (1);
    }
	type = determine_token_type(value, expect_command);
	add_token(tokens, create_token(type, value));
	free(value);
	return (0);
}

// pipes
int	pipe_handling(const char *input, int *i, t_token **tokens)
{
	size_t	start;
	char	*value;

	while (input[*i] && ft_isspace(input[*i]))
		(*i)++;
	if (!input[*i] || (input[*i] == '|' && (input[*i + 1] == '\0')))
	{
		ft_printf_fd(2, OPEN_PIPE);
		return (1);
	}
	start = *i;
	(*i)++;
	value = ft_substr(input, start, *i - start);
	if (!value)
		return (1);
	add_token(tokens, create_token(PIPE, value));
	free(value);
	while (input[*i] && ft_isspace(input[*i]))
		(*i)++;
	return (0);
}


int	redirect_handling(t_token *tokens, t_shell *shell)
{
    int		fd_out;
    t_token	*temp;
    int		heredoc_fd[2];
    char    *heredoc_content;

    fd_out = -1;
    temp = tokens;
    heredoc_content = NULL;
    while (temp != NULL)
    {
        if (temp->type == HERE_DOC && temp->next && temp->next->type == DELIMETER)
        {
            if (heredoc_content)
                free(heredoc_content);
            heredoc_content = read_heredoc_input(temp->next->value, temp->next->expand_heredoc, shell);
            if (!heredoc_content)
                return (-1);
            temp = temp->next->next;
        }
        else if (temp->type == REDIRECT && temp->next && temp->next->type == FILES)
        {
            if (ft_strcmp(temp->value, ">") == 0)
            {
                if (fd_out != -1)
                    close(fd_out);
                fd_out = open(temp->next->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (fd_out == -1)
                    return (-1);
                dup2(fd_out, STDOUT_FILENO);
                close(fd_out);
            }
            temp = temp->next->next;
        }
        else
            temp = temp->next;
    }
    if (heredoc_content)
    {
        if (pipe(heredoc_fd) == -1)
        {
            free(heredoc_content);
            return (-1);
        }
        write (heredoc_fd[1], heredoc_content, ft_strlen(heredoc_content));
        close(heredoc_fd[1]);
        dup2(heredoc_fd[0], STDIN_FILENO);
        close(heredoc_fd[0]);
        free(heredoc_content);
    }
    return (0);
}

int	heredoc_handling(const char *input, int *i, t_token **tokens)
{
    char	*delimiter;
    int		start;
    int		expand;
    t_token	*heredoc_token;
    t_token	*delimiter_token;

    (*i) += 2;
    while (input[*i] && ft_isspace(input[*i]))
        (*i)++;
    start = *i;
    while (input[*i] && !ft_isspace(input[*i]) && input[*i] != '|'
        && input[*i] != '<' && input[*i] != '>')
        (*i)++;
    delimiter = ft_substr(input, start, *i - start);
    if (!delimiter)
        return (1);
    expand = 1;
    if ((delimiter[0] == '\'' && delimiter[ft_strlen(delimiter) - 1] == '\'')
        || (delimiter[0] == '"' && delimiter[ft_strlen(delimiter) - 1] == '"'))
    {
        char *temp = ft_strtrim(delimiter, "\'\"");
        free(delimiter);
        delimiter = temp;
        expand = 0;
    }
    heredoc_token = create_token(HERE_DOC, "<<");
    add_token(tokens, heredoc_token);
    delimiter_token = create_token(DELIMETER, delimiter);
    delimiter_token->expand_heredoc = expand;
    add_token(tokens, delimiter_token);
    free(delimiter);
    return (0);
}

// token handling
int	token_handling(const char *input, int *i, t_token **tokens,
        int *expect_command)
{
    if (!input || !input[*i])
        return (1);
    while (input[*i] && !ft_isspace(input[*i]))
    {
        if (!input[*i])
            break ;
        if (input[*i] == '\'' || input[*i] == '"')
        {
            if (quote_handling(input, i, tokens, expect_command))
                return (1);
            break;
        }
        if (input[*i] == '|' || input[*i] == '<' || input[*i] == '>')
        {
            if (special_tokens_handling(input, i, tokens, expect_command))
                return (1);
            *expect_command = 1;
            while (input[*i] && ft_isspace(input[*i]))
                (*i)++;
        }
        else if (input[*i] == '#')
            return (2);
        else if (ft_isprint(input[*i]))
        {
            if (*expect_command && !ft_isdigit(input[*i]))
            {
                if (word_handling(input, i, tokens, expect_command))
                    return (1);
            }
            else
            {
                if (word_handling(input, i, tokens, expect_command))
                    return (1);
            }
            break; // Added break to exit inner loop after processing a word
        }
        else
            (*i)++;
        if (input[*i] && ft_isspace(input[*i]))
            break ;
    }
    return (0);
}
