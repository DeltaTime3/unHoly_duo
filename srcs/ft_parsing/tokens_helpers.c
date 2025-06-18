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
	return (0);
}

// operators
int	op_handling(const char *input, int *i, t_token **tokens)
{
	size_t	start;
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
	(*i)++;
	while (input[*i] && ft_isspace(input[*i]))
		(*i)++;
	if (input[*i] && file_handling(input, i, tokens))
		return (1);
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
	return (0);
}

// pipes
int	pipe_handling(const char *input, int *i, t_token **tokens)
{
	size_t	start;
	char	*value;

	while (input[*i] && ft_isspace(input[*i]))
		(*i)++;
	if (!input[*i] || (input[*i] == '|' && (input[*i + 1] == '\0' || input[*i + 2] == ' ')))
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
	while (input[*i] && ft_isspace(input[*i]))
		(*i)++;
	free(value);
	return (0);
}

int	redirect_handling(t_token *tokens)
{
    int	fd_out;
	int	fd_in;
	int	temp_fd;
	char	*last_out_file;
	char	*last_append_file;
	char	*last_in_file;
	int		out_mode;
	t_token	*temp;

	fd_out = -1;
	fd_in = -1;
	last_out_file = NULL;
	last_append_file = NULL;
	last_in_file = NULL;
	out_mode = 0;
	temp = tokens;
	// collect the last redirection of each type
	while (temp)
	{
		if (temp->type == REDIRECT)
		{
			if (!temp->next || temp->next->type != FILES)
			{
				ft_printf_fd(2, UNEXPECTED_TOKEN);
				return (-1);
			}
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
				last_in_file = temp->next->value;
		}
		temp = temp->next;
	}
	// open files based on last redirection of each type
	if (last_out_file && out_mode != 1)
	{
		temp_fd = open(last_out_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (temp_fd == -1)
			return (print_error("Error opening file for output redirection\n"), -1);
		close(temp_fd);
	}
	else if (last_append_file && out_mode != 2)
	{
		temp_fd = open(last_append_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (temp_fd == -1)
			return (print_error("Error opening file for append redirection\n"), -1);
		close(temp_fd);
	}
	if (out_mode == 1 && last_out_file)
    {
        fd_out = open(last_out_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd_out == -1)
            return (print_error("Error opening file for output redirection\n"), -1);
    }
    else if (out_mode == 2 && last_append_file)
    {
        fd_out = open(last_append_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (fd_out == -1)
            return (print_error("Error opening file for append redirection\n"), -1);
	}
	if (last_in_file)
	{
		fd_in = open(last_in_file, O_RDONLY);
		if (fd_in == -1)
			return (print_error("Error opening file for input redirection\n"), -1);
	}
	// apply redirections
	if (fd_out != -1)
	{
		dup2(fd_out, STDOUT_FILENO);
		close(fd_out);
	}
	if (fd_in != -1)
	{
		dup2(fd_in, STDIN_FILENO);
		close(fd_in);
	}
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
		if (input[*i] == '|' || input[*i] == '<' || input[*i] == '>')
		{
			if (special_tokens_handling(input, i, tokens, expect_command))
				return (1);
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
		}
		else
			(*i)++;
		if (input[*i] && ft_isspace(input[*i]))
			break ;
	}
	return (0);
}
