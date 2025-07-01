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

int	redirect_handling(t_token *tokens)
{
    int	fd_out = -1;
    int	fd_in = -1;
    char	*last_out_file = NULL;
    char	*last_append_file = NULL;
    char	*last_in_file = NULL;
    int	out_mode = 0;
    t_token	*temp = tokens;
    
    // Process all redirections
    while (temp)
    {
        if (temp->type == HERE_DOC && temp->next && temp->next->type == DELIMETER)
        {
            char *heredoc_content = read_heredoc_input(temp->next->value);
            if (!heredoc_content)
                return (-1);
            
            int heredoc_fd[2];
            if (pipe(heredoc_fd) == -1)
            {
                free(heredoc_content);
                return (-1);
            }
            
            // Write content to pipe
            write(heredoc_fd[1], heredoc_content, ft_strlen(heredoc_content));
            close(heredoc_fd[1]);
            
            // Redirect stdin to read from pipe
            dup2(heredoc_fd[0], STDIN_FILENO);
            close(heredoc_fd[0]);
            
            free(heredoc_content);
            temp = temp->next; // Skip delimiter
        }
        else if (temp->type == REDIRECT)
        {
            if (temp->next && temp->next->type == FILES)
            {
                if (ft_strcmp(temp->value, ">") == 0)
                {
                    last_out_file = temp->next->value;
                    out_mode = 1;
                }
                else if (ft_strcmp(temp->value, ">>") == 0)
                {
                    last_append_file = temp->next->value;
                    out_mode = 2;
                }
                else if (ft_strcmp(temp->value, "<") == 0)
                {
                    last_in_file = temp->next->value;
                }
                temp = temp->next; // Skip the file token
            }
            else
            {
                ft_printf_fd(2, UNEXPECTED_TOKEN);
                return (-1);
            }
        }
        // Remove the else clause that was catching all other tokens
        temp = temp->next;
    }
    
    // Handle other redirections after processing all tokens
    if (out_mode == 1 && last_out_file)
    {
        fd_out = open(last_out_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd_out == -1)
            return (-1);
        dup2(fd_out, STDOUT_FILENO);
        close(fd_out);
    }
    else if (out_mode == 2 && last_append_file)
    {
        fd_out = open(last_append_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (fd_out == -1)
            return (-1);
        dup2(fd_out, STDOUT_FILENO);
        close(fd_out);
    }
    
    if (last_in_file)
    {
        fd_in = open(last_in_file, O_RDONLY);
        if (fd_in == -1)
            return (-1);
        dup2(fd_in, STDIN_FILENO);
        close(fd_in);
    }
    
    return (0);
}

int	heredoc_handling(const char *input, int *i, t_token **tokens)
{
	char	*delimiter;
	int		start;

	(*i) += 2;
	while (input[*i] && ft_isspace(input[*i]))
		(*i)++;
	// extract delimeter
	start = *i;
	while (input[*i] && !ft_isspace(input[*i]) && input[*i] != '|'
			&& input[*i] != '<' && input[*i] != '>')
			(*i)++;
	delimiter = ft_substr(input, start, *i - start);
	if (!delimiter)
		return (1);
	add_token(tokens, create_token(HERE_DOC, "<<"));
	add_token(tokens, create_token(DELIMETER, delimiter));
	free(delimiter);
	while (input[*i] && ft_isspace(input[*i]))
		(*i)++;
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
            break; // Changed from return(0) to break to continue processing
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
            break; // Added break to exit inner loop after processing a word
        }
        else
            (*i)++;
        if (input[*i] && ft_isspace(input[*i]))
            break ;
    }
    return (0);
}
