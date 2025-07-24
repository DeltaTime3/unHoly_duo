#include "../../include/minishell.h"

t_env	*find_env_node(t_env *head, char *key)
{
	t_env	*node;

	node = head;
	while (node)
	{
		if (ft_strcmp(node->key, key) == 0)
			return (node);
		node = node->next;
	}
	return (NULL);
}

void	update_env_value_bi(t_env *env, char *value, int flag)
{
	char	*new_value;

	if (!env)
		return ;
	if (flag == 2)
	{
		new_value = malloc(ft_strlen(env->value) + ft_strlen(value) + 1);
		if (!new_value)
			return ;
		ft_strcpy(new_value, env->value);
		ft_strcat(new_value, value);
		free(env->value);
		env->value = new_value;
	}
	else
	{
		free(env->value);
		env->value = ft_strdup(value);
	}
	if (flag)
		env->flag = 1;
}

void	print_env(t_shell *type)
{
	t_env	*node;

	node = type->head;
	while (node)
	{
		printf("%s=%s\n", node->key, node->value);
		node = node->next;
	}
}

char	*extract_key(const char *args, int len)
{
	char	*key;

	key = ft_calloc(1, len + 1);
	if (!key)
		return (NULL);
	ft_strlcpy(key, args, len + 1);
	return (key);
}

void	append_env_value(t_env *env, char *value)
{
	char	*new_val;

	if (!env)
		return ;
	if (env->value && *env->value)
	{
		new_val = ft_calloc(1, ft_strlen(env->value) + ft_strlen(value) + 1);
		if (!new_val)
			return ;
		ft_strlcpy(new_val, env->value, ft_strlen(env->value));
		ft_strlcat(new_val, env->value, ft_strlen(env->value));
	}
	else
	{
		new_val = ft_strdup(value);
	}
	free(env->value);
	env->value = new_val;
}
