/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppaula-d <ppaula-d@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 13:59:12 by ppaula-d          #+#    #+#             */
/*   Updated: 2025/07/31 23:30:01 by ppaula-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_dir(char *new_dir)
{
	struct stat	path_status;

	if (!new_dir)
	{
		print_error(E_HOME);
		return (0);
	}
	if (stat(new_dir, &path_status) == -1)
	{
		print_error(E_CD);
		return (0);
	}
	if (!S_ISDIR(path_status.st_mode))
	{
		print_error(E_CD);
		return (0);
	}
	if (access(new_dir, R_OK | X_OK) == -1)
	{
		print_error(E_PERMIT);
		return (0);
	}
	return (1);
}

void	cd_env_pwd(t_shell *type)
{
	t_env	*new_pwd;
	char	cwd[MAX_PATH];

	if (!getcwd(cwd, sizeof(cwd)))
	{
		print_error("cd: error retriving current directory\n");
		return ;
	}
	new_pwd = find_env_node(type->head, "PWD");
	if (new_pwd)
	{
		free(new_pwd->value);
		new_pwd->value = ft_strdup(cwd);
	}
	else
	{
		add_env_node(&type->head, "PWD", cwd, 1);
	}
	free(type->pwd);
	type->pwd = ft_strdup(cwd);
}

void	cd_env(t_shell *type)
{
	t_env	*curr;

	curr = type->head;
	while (curr)
	{
		if (ft_strcmp(curr->key, "OLDPWD=") == 0)
		{
			free(curr->value);
			curr->value = ft_strdup(type->prev_dir);
			cd_env_pwd(type);
			return ;
		}
		curr = curr->next;
	}
	add_old_pwd_to_env(type);
	cd_env_pwd(type);
}

void	add_old_pwd_to_env(t_shell *type)
{
	t_env	*node;

	node = find_env_node(type->head, "OLDPWD");
	if (node)
	{
		free(node->value);
		node->value = ft_strdup(type->prev_dir);
	}
	else
	{
		add_env_node(&type->head, "OLDPWD", type->prev_dir, 1);
	}
}

int	ct_nodes(t_token *token)
{
	int	argc;

	argc = 0;
	if (!token || !token->args)
		return (0);
	while (token->args[argc])
		argc++;
	return (argc);
}
