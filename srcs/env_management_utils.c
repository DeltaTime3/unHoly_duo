#include "../include/minishell.h"

void	update_env_value(t_env *node, char *value, int append)
{
	char	*new_val;

	if (append)
	{
		new_val = append_env_value_man(node->value, value);
		if (!new_val)
			return ;
		free(node->value);
		node->value = new_val;
	}
	else
	{
		free(node->value);
		if (value)
		{
			node->value = ft_strdup(value);
		}
		else
		{
			node->value = NULL;
		}
	}
}

void	add_new_node(t_env **head, t_env *tail, char *key, char *value)
{
	t_env	*new_node;

	new_node = create_node_from_key(key, value);
	if(!new_node)
		return ;
	if (!*head)
	{
		*head = new_node;
		if (tail)
			tail = new_node;
	}
	else
	{
		if (tail && tail)
		{
			tail->next = new_node;
			tail = new_node;
		}
	}
}

void	free_env_node(t_env *node)
{
	if(!node)
		return ;
	free(node->key);
	free(node->value);
	free(node);
}

void free_env_array(char **env)
{
	int	i;
	
	i = 0;
	if (!env)
		return ;
	while(env[i])
	{
		free(env[i]);
		i++;
	}
	free(env);
}
void	free_env(t_env *head)
{
	t_env	*temp;

	while (head)
	{
		temp = head;
		head = head->next;
		if (temp->key)
			free(temp->key);
		if (temp->value)
			free(temp->value);
		free(temp);
	}
}

char	**env_list_to_array(t_env *head)
{
	char 	**array;
	char	*temp;
	int		i;
	t_env	*curr;

	i = 0;
	curr = head;
	while (curr)
	{
		if (curr->value) // Only count entries with values
            i++;
        curr = curr->next;
	}
	array = ft_calloc(i + 1, sizeof(char *));
	if (!array)
		return (NULL);
	curr = head;
	i = 0;
	while (curr)
	{
		if (curr->value)
		{
			temp = ft_strjoin(curr->key, "=");
			array[i] = ft_strjoin(temp, curr->value);
			free(temp);
			curr = curr->next;
			i++;
		}
	}
	array[i] = NULL;
	return(array);
}

void	clean_all_resources(t_shell *shell)
{
    
    if (shell->head)
    {
        free_env(shell->head);
        shell->head = NULL;
    }
    // Free all dynamically allocated shell fields
    if (shell->prev_dir)
    {
        free(shell->prev_dir);
        shell->prev_dir = NULL;
    }
    if (shell->curr_dir)
    {
        free(shell->curr_dir);
        shell->curr_dir = NULL;
    }
    if (shell->pwd)
    {
        free(shell->pwd);
        shell->pwd = NULL;
    }
    if (shell->token)
    {
        free(shell->token);
        shell->token = NULL;
    }
    if (shell->type)
    {
        free(shell->type);
        shell->type = NULL;
    }
    if (shell->value)
    {
        free(shell->value);
        shell->value = NULL;
    }
    // Null out pointers for safety
    shell->tail = NULL;
}

void	clean_command_resources(t_shell *shell)
{
	if (shell->token)
	{
		free(shell->token);
		shell->token = NULL;
	}
	if (shell->type)
	{
		free(shell->type);
		shell->type = NULL;
	}
	if (shell->value)
	{
		free(shell->value);
		shell->value = NULL;
	}
}
