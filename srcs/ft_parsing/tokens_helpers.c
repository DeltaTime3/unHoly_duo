#include "minishell.h"

// files
static char *ft_strjoin_free(char *s1, char *s2)
{
    char *res = ft_strjoin(s1, s2);
    free(s1);
    return res;
}

int file_handling(const char *input, int *i, t_token **tokens)
{
    char    *value;
    char    *tmp;
    size_t  start;
    char    quote;

    // skip leading spaces
    while (input[*i] && ft_isspace(input[*i]))
        (*i)++;
    // init empty accumulator
    value = ft_strdup("");
    if (!value)
        return (1);
    // loop until next whitespace
    while (input[*i] && !ft_isspace(input[*i]) && input[*i] != '|' && input[*i] != '<' && input[*i] != '>')
    {
        if (input[*i] == '"' || input[*i] == '\'')
        {
            quote = input[(*i)++];
            start = *i;
            while (input[*i] && input[*i] != quote)
                (*i)++;
            tmp = ft_substr(input, start, *i - start);
            if (input[*i] == quote)
                (*i)++;
        }
        else
        {
            start = *i;
            while (input[*i] && !ft_isspace(input[*i])
                   && input[*i] != '"' && input[*i] != '\'' 
                   && input[*i] != '|' && input[*i] != '<' && input[*i] != '>')
                (*i)++;
            tmp = ft_substr(input, start, *i - start);
        }
        if (!tmp)
            return (free(value), 1);
        // append to accumulator
        value = ft_strjoin_free(value, tmp);
        free(tmp);
    }
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

int	word_handling(const char *input, int *i, t_token **tokens,
		int *expect_command)
{
	int		start;
	char	quote_char;
	char	*content;
	t_cat	type;

	start = *i;
	quote_char = 0;
	while (input[*i])
	{
		if (quote_char)
		{
			if (input[*i] == quote_char)
				quote_char = 0; // Found closing quote
		}
		else
		{
			if (input[*i] == '\'' || input[*i] == '"')
				quote_char = input[*i]; // Found opening quote
			// Break if we hit a metacharacter or whitespace NOT inside quotes
			else if (ft_isspace(input[*i]) || input[*i] == '|'
				|| input[*i] == '<' || input[*i] == '>')
				break ;
		}
		(*i)++;
	}
	content = ft_substr(input, start, *i - start);
	if (!content)
		return (1); // Malloc failure
	type = determine_token_type(content, expect_command);
	add_token(tokens, create_token(type, content));
	free(content);
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

int redirect_handling(t_token *tokens, t_shell *shell)
{
    t_token *temp = tokens;
    int fd_in = -1;
    int fd_out = -1;
    char *heredoc_content;

    while (temp && temp->type != PIPE)
    {
        if (temp->type == REDIRECT && temp->next && temp->next->type == FILES)
        {
            if (ft_strcmp(temp->value, "<") == 0)
            {
                if (fd_in != -1)
                    close(fd_in);
                fd_in = open(temp->next->value, O_RDONLY);
                if (fd_in == -1)
                {
                    perror(temp->next->value);
                    shell->exit_code = 1;
                    if (fd_out != -1) close(fd_out);
                    return (-1);
                }
            }
            else if (ft_strcmp(temp->value, ">") == 0 || ft_strcmp(temp->value, ">>") == 0)
            {
                if (fd_out != -1)
                    close(fd_out);
                int flags = O_WRONLY | O_CREAT;
                flags |= (ft_strcmp(temp->value, ">>") == 0) ? O_APPEND : O_TRUNC;
                fd_out = open(temp->next->value, flags, 0644);
                if (fd_out == -1)
                {
                    perror(temp->next->value);
                    shell->exit_code = 1;
                    if (fd_in != -1) close(fd_in);
                    return (-1);
                }
            }
            temp = temp->next; // Skip the filename token
        }
        else if (temp->type == HERE_DOC && temp->next && temp->next->type == DELIMETER)
        {
            if (fd_in != -1)
                close(fd_in);
            heredoc_content = read_heredoc_input(temp->next->value, temp->next->expand_heredoc, shell);
            if (!heredoc_content)
            {
                shell->exit_code = 130; // Standard exit code for SIGINT
                if (fd_out != -1) close(fd_out);
                return (-1);
            }
            int heredoc_fd[2];
            if (pipe(heredoc_fd) == -1)
            {
                free(heredoc_content);
                perror("pipe");
                shell->exit_code = 1;
                if (fd_out != -1) close(fd_out);
                return (-1);
            }
            write(heredoc_fd[1], heredoc_content, ft_strlen(heredoc_content));
            close(heredoc_fd[1]);
            free(heredoc_content);
            fd_in = heredoc_fd[0];
            temp = temp->next; // Skip the delimiter token
        }
        if (temp)
            temp = temp->next;
    }

    if (fd_in != -1)
    {
        if (dup2(fd_in, STDIN_FILENO) == -1)
        {
            perror("dup2 for stdin");
            close(fd_in);
            if (fd_out != -1) close(fd_out);
            return (-1);
        }
        close(fd_in);
    }
    if (fd_out != -1)
    {
        if (dup2(fd_out, STDOUT_FILENO) == -1)
        {
            perror("dup2 for stdout");
            close(fd_out);
            return (-1);
        }
        close(fd_out);
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

int	token_handling(const char *input, int *i, t_token **tokens,
		int *expect_command)
{
	if (!input || !input[*i])
		return (1); // End of input
	if (input[*i] == '|' || input[*i] == '<' || input[*i] == '>')
	{
		if (special_tokens_handling(input, i, tokens, expect_command))
			return (1);
		if (input[*i - 1] == '|')
			*expect_command = 1;
	}
	else if (input[*i] == '#')
	{
		while (input[*i] && input[*i] != '\n')
			(*i)++;
	}
	else if (ft_isprint(input[*i]) && !ft_isspace(input[*i]))
	{
		if (word_handling(input, i, tokens, expect_command))
			return (1);
	}
	else
	{
		(*i)++; // Skip whitespace or other non-handled characters
	}
	return (0);
}

