/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-d <ppaula-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 16:08:35 by afilipe-          #+#    #+#             */
/*   Updated: 2025/06/25 14:36:15 by ppaula-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_env	*find_env_node(t_env *head, char *key)
{
	t_env *node;

	node = head;
	while (node)
	{
		if (ft_strcmp(node->key, key) == 0)
			return (node);
		node = node->next;
	}
	return (NULL);
}

void 	update_env_value_bi(t_env *env, char *value, int flag)
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
	t_env *node;
	
	node = type->head;
	while (node)
	{
		printf("%s=%s\n", node->key, node->value);
		node = node->next;
	}
}

char 	*extract_key(const char *args, int len)
{
	char *key;

	key = ft_calloc(1, len + 1);
	if (!key)
		return (NULL);
	ft_strlcpy(key, args, len + 1);
	return (key);
}
void 	append_env_value(t_env *env, char *value)
{
	char	*new_val;
	
	if (!env)
		return;
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


char    *expand_token_arg_to_value(char *value, t_shell *shell)
{
    char	*result;
    char	*temp;
    char	*home;
	char	*expan;

	if (ft_strchr(value, '$'))
		{
			expan = expand_command_arg(value, shell);
				return (expan);
		}
    if (value[0] == '~' && (value[1] == '\0' || value[1] == '/'))
    {
        home = get_env_value(shell->head, "HOME");
        if (home)
        {
            temp = ft_strjoin(home, value + 1);
            result = temp;
        }
        else
            return (ft_strdup(value));
    }
    else if (ft_strcmp(value, "$?") == 0)
        result = expand_exit_status(value, shell);
    else if (value[0] == '$' && value[1] != '\0')
        result = expand_env_var(value, shell);
    else
        result = ft_strdup(value);
    return (result);
}

char	*expand_command_arg(const char *imput, t_shell *shell)
{
	char	*result;
	char	*var_value;
	char	*temp;
	int		i;

	i = 0;
	result = ft_strdup("");
	while (imput[i])
	{
		if (imput[i] == '$' && imput[i + 1] != '\0' && imput[i + 1] != ' ')
		{
			var_value = expand_env_var(&imput[i], shell);
			temp = ft_strjoin(result, var_value);
			free(result);
			free(var_value);
			result = temp;
			while (imput[i] && !ft_isspace(imput[i]) && imput[i] != '=')
				i++;
		}
		else
		{
			temp = ft_strjoin(result, &imput[i]);
			free(result);
			result = temp;
			i++;
		}
	}
	return (result);
}
