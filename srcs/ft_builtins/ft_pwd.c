/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afilipe- <afilipe-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 11:51:47 by afilipe-          #+#    #+#             */
/*   Updated: 2025/06/25 14:00:54 by afilipe-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_pwd(t_shell *type)
{
	char	cwd[MAX_PATH];

	if (!type)
		return (1);
	if (type->pwd)
	{
		printf("%s\n", type->pwd);
		type->exit_code = 0;
		return (0);
	}
	else
	{
		return (pwd_ut(cwd, type));
	}
}

int	pwd_ut(char cwd[MAX_PATH], t_shell *type)
{
	if (getcwd(cwd, MAX_PATH))
		{
			printf("%s\n", cwd);
			type->exit_code = 0;
			return (0);
		}
		else
		{
			perror("pwd error: directory not set.\n");
			type->exit_code = 1;
			return (1);
		}
}
