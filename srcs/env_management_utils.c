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

	while(head)
	{
		temp = head->next;
		free(head->key);
		head->key = NULL;
		free(head->value);
		head->value = NULL;
		free(head);
		head = temp;
	}
}

char	**env_list_to_array(t_env *head)
{
	char 	**array;
	int		count;

	count = count_env(head);
	array = ft_calloc(count + 1, sizeof(char *));
	if (!array)
		return (NULL);
	fill_env_array(array, head);
	return(array);
}

int	count_env(t_env *head)
{
	int	count;
	t_env	*curr;

	count = 0;
	curr = head;

	while (curr)
	{
		if (curr->value)
			count++;
		curr = curr->next;
	}
	return (count);
}

void	fill_env_array(char **array, t_env *head)
{
	int	i;
	char	*temp;
	t_env	*curr;

	i = 0;
	curr = head;
	while (curr)
	{
		if (curr->value)
		{
			temp = ft_strjoin(curr->key, "=");
			array[i] = ft_strjoin(temp, curr->value);
			free(temp);
			i++;
		}
		curr = curr->next;
	}
	array[i] = NULL;
}

void	clean_all_resources(t_shell *shell)
{
    
    clean_env(shell);
	clean_directories(shell);
	clean_token_type_value(shell);
	shell->tail = NULL;
}

void	clean_env(t_shell *shell)
{
	if (shell->head)
	{
		free_env(shell->head);
		shell->head = NULL;
	}
}

void	clean_directories(t_shell *shell)
{
	free_and_null((void **)&(shell->prev_dir));
	free_and_null((void **)&(shell->curr_dir));
	free_and_null((void **)&(shell->pwd));
}

void	clean_token_type_value(t_shell *shell)
{
	free_and_null((void **)&(shell->token));
	free_and_null((void **)&(shell->type));
	free_and_null((void **)&(shell->value));
}

void	free_and_null(void **ptr)
{
	if (ptr && *ptr)
	{
		free(*ptr);
		*ptr = NULL;
	}
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
