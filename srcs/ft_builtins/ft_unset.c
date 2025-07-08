#include "../../include/minishell.h"

/**
 * Attempts to remove env variables listed in the token list.
 * for each argument in the token list, it checks if the argument 
 * is a valid identifier.
 * if vadid, removes the variable from the environment.
 * if invalid, sets error code to 1 but continues processing the rest of the 
 * arguments.
 * sets shell->return_code to the error code.(0 for success, 1 for error).
 * @param shell pointer to shell Struct
 * @param args linked list of tokens.
 * @return 0 if successful, 1 if there was an error with any of the arguments.
*/
int	ft_unset(t_shell *shell, t_token *args)
{
	int		error_code;
	t_token	*curr;

	error_code = 0;
	curr = args;
	while (curr)
	{
		if (!validate_unset_args(curr->value))
		{
			error_code = 0;
		}
		else
		{
			env_remove(&shell->head, curr->value);
		}
		curr = curr->next;
	}
	shell->return_code = error_code;
	return (error_code);
}
/**
 * checks the validity of the argument for unset.
 * the fisrt char must be an alphabetic char or an '_"
 * the rest of the string must be alphanumeric or '_'.
 * returns 1 if valid, 0 if invalid.
 */

int	validate_unset_args(char *args)
{
	int	i;

	if (!args || !*args)
		return (0);
	if (!ft_isalpha(args[0]) && args[0] != '_')
		return (0);
	i = 1;
	while (args[i])
	{
		if (!ft_isalnum(args[i]) && args[i] != '_')
			return (0);
		i++;
	}
	return (1);
}
