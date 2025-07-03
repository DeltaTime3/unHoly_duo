
#include "minishell.h"

char    *read_heredoc_input(const char *delimiter)
{
    char    *line;
    char    *content;
    char    *temp;
    
	signal(SIGINT, handle_sig_heredoc);
    content = ft_strdup("");
    if (!content)
        return (NULL);
    while (1)
    {
		line = readline("> ");
        if (!line || global_sig	== 130)
        {
            if(line)
				free(line);
			free(content);
            return (NULL);
        }
        // Simple string comparison without trimming
        if (ft_strcmp(line, delimiter) == 0)
        {
            free(line);
            free(content);
            return (0);
        }
        // append line to content with newline
        temp = ft_strjoin(content, line);
        free(content);
        if (!temp)
        {
            free(line);
            return (NULL);
        }
        content = temp;
        temp = ft_strjoin(content, "\n");
		free(content);
        if (!temp)
        {
            free(line);
            return (NULL);
        }
        content = temp;
        free(line);
    }
	signal(SIGINT, handle_sig_int);
    return (content);
}
