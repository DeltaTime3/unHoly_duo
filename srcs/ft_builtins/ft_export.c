/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afilipe- <afilipe-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 11:51:25 by afilipe-          #+#    #+#             */
/*   Updated: 2025/06/09 14:55:42 by afilipe-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_export(t_shell *type, char **args)
{
	int		status;
	int		i;
	
	i = 1;
	status = 0;
	if (!args[1])
		return(print_export_list(type->head), 0);
	while (args[i])
	{
		if (validate_args(args[i]))
		{
			ft_fprintf(2, "minishell: export: `%s': %s\n", args[i], E_INVALID_ID);
			status = 1;
		}
		else
			process_export(type, args[i]);
		i++;
	}
	return (status);
}

int	validate_args(char *args)
{
	if(val_empt_operat(args) || val_fst_char(args) ||
		val_var_name(args))
		{
			print_error(E_INPUT);
			return (1);
		}
	return (0);
}

int	val_fst_char(const char *args)
{
	if (!ft_isalpha(args[0]) && args[0] != '_')
		return (1);
	return (0);
}

int	val_var_name(const char *args)
{
	int	j;

	j = 0;
	while ((args[j] && args[j] != '=') 
		&& !(args[j] == '+' && args[j + 1]== '='))
		{
			if (!ft_isalnum(args[j]) && args[j] != '_')
				return (1);
			j++;
		}
		return (0);
}

int	val_empt_operat(const char *args)
{
	if (args[0] == '\0' || !ft_strcmp(args, "=") || !ft_strcmp(args, "+="))
		return (1);
	return (0);
}

/**
 * ERRORS
 * Everything that doesn't start with char upper or lower
 * or '_'
 * before '=' you can have '+' and/or numbers;
 * "export a+=12" is valid but "export a+a=12" is not
 * "export a1=12" is vadid but "export 1a=12" is not
 * after '=' all ascii symbols are vadid
 * space make a new argsument so "a+= 12" the error is the 12,
 * but "a+=12 b+=45" is valid, but "a+=12 += b+=42" is not "+= is error"
 * it will run until the end note and skip the errors and run the valids; 
 */
