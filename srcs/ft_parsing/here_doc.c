
#include "minishell.h"

#include "minishell.h"

char    *read_heredoc_input(const char *delimiter, int expand, t_shell *shell)
{
    char    *line;
    char    *content;
    char    *temp;
    
	global_sig = 0;
	signal(SIGINT, handle_sig_heredoc);
    content = ft_strdup("");
    if (!content)
	{
		signal(SIGINT, handle_sig_int);
		return (NULL);
	}
    while (1)
    {
		line = readline("> ");
        if (!line || global_sig	== 1)
        {
			free(line);
			free(content);
			signal(SIGINT, handle_sig_int);
            return (NULL);
        }
        if (ft_strcmp(line, delimiter) == 0)
        {
            free(line);
            break;
        }
        if (expand)
        {
            char *expanded_line = expand_token_value(line, shell);
            free(line);
            line = expanded_line;
        }
        temp = ft_strjoin(content, line);
        free(content);
        if (!temp)
        {
            free(line);
			signal(SIGINT, handle_sig_int);
            return (NULL);
        }
        content = temp;
        temp = ft_strjoin(content, "\n");
		free(content);
        if (!temp)
        {
            free(line);
			signal(SIGINT, handle_sig_int);
            return (NULL);
        }
        content = temp;
        free(line);
    }
    signal(SIGINT, handle_sig_int);
    return (content);
}
