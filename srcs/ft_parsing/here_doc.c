
#include "minishell.h"

char    *read_heredoc_input(const char *delimiter)
{
    char    *line;
    char    *content;
    char    *temp;
    
    content = ft_strdup("");
    if (!content)
        return (NULL);
    while (1)
    {
        line = readline("> ");
        if (!line)
        {
            free(content);
            return (NULL);
        }
        // Simple string comparison without trimming
        if (ft_strcmp(line, delimiter) == 0)
        {
            free(line);
            //free(content);
            return (content);
        }
        // append line to content with newline
        temp = ft_strjoin(content, line);
        free(content);
        if (!temp)
        {
            free(line);
            ft_signals();
            return (NULL);
        }
        content = temp;
        temp = ft_strjoin(content, "\n");
        if (!temp)
        {
            free(line);
            ft_signals();
            return (NULL);
        }
        free(content);
        content = temp;
        free(line);
    }
    return (content);
}