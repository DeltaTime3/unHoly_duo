#include "../include/minishell.h"

/**
 * Initializes a linked list of env variables from envp array;
 * returns the head of the list;
 * create a new node for the current env variable string;
 * on allocation fail, free all prev alocated nodes
 * set head for the first node;
 * link the node at the end of list;
 */
t_env	*init_env(char **envp)
{
	t_env	*head;
	t_env	*new;
	t_env	*tail;

	head = NULL;
	tail = NULL;
	while (*envp)
	{
		new = create_env_node(*envp);
		if (!new)
		{
			free_env(head);
			return (NULL);
		}
		if (!head)
			head = new;
		else
			tail->next = new;
		tail = new;
		envp++;
	}
	return (head);
}

/**
 * create a single env var node from a key=value string.
 * retuns a pointes to the new node, or NULL on alloc fail
 * find '=' to split key and value
 * if no '=' is found treat the str as key, value is NULL;
 * 
 */
t_env	*create_env_node(char *str)
{
	t_env	*node;
	char	*match;

	if(!str)
		return (NULL);
	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	match = ft_strchr(str, '=');
	if(!match)
	{
		node->key = ft_strdup(str);
		node->value = NULL;
	}
	else
	{
		node->key = ft_substr(str, 0, match - str);
		node->value = ft_strdup(match + 1);
	}
	if(!node->key || (match && !node->value))
		return (free_env_node(node), NULL);	
	node->next = NULL;
	return (node);
}

/**
 * removes env variables with the given key
 * frees memory;
 * compares keys to find the node to remove
 */
void	env_remove(t_env **head, char *key)
{
	t_env	*curr;
	t_env	*prev;

	curr = *head;
	prev = NULL;
	while (curr)
	{
		if (ft_strcmp(curr->key, key) == 0)
		{
			if (prev)
				prev->next = curr->next;
			else
				*head = curr->next;
			free_env_node(curr);
			return;
		}
	prev = curr;
	curr = curr->next;
	}
}
/**
 * updates the value of an env variable in the list
 * if append isn't zero, apends valeu to existing value
 * if key doesn't exit, creates it as a new node at the head of list
 */
void	update_env(t_env **head, char *key, char *value, int append)
{
	t_env	*curr;
	t_env	*tail;

	curr = *head;
	tail = NULL;
	while (curr)
	{
		if (ft_strcmp(curr->key, key) == 0)
		{
			update_env_value(curr, value, append);
			return ;
		}
		tail = curr;
		curr = curr->next;
	}
	add_new_node(head,tail, key, value);
}

t_env *create_node_from_key(char *key, char *value)
{
	t_env	*node;
	
	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = ft_strdup(key);
	if (value)
		node->value = ft_strdup(value);
	else
		node->value = NULL;
	if (!node->key || (value && !node->value))
	{
		free_env_node(node);
		return (NULL);
	}
	node->next = NULL;
	return (node);
}

char	*append_env_value_man(char *old_val, char *add_val)
{
	size_t	len_old;
	size_t	len_add;
	char	*new_val;

	len_old = 0;
	len_add = 0;
	if (old_val)
		len_old = ft_strlen(old_val);
	if (add_val)
		len_add = ft_strlen(add_val);
	new_val = malloc(len_old +len_add + 1);
	if (!new_val)
		return(NULL);
	if (old_val)
		ft_strlcpy(new_val, old_val, len_old + 1);
	else
		new_val[0] = '\0';
	if (add_val)
		ft_strlcat(new_val, add_val, len_old + len_add + 1);
	return (new_val);
}
