#include "../../include/minishell.h"

/**
 * retrieves the value of an environment variable, from the shellstate,
 * searches the shell env for the var to search
 * if found, returns a pointer to the value part of the env variable.
 * returns pointer to 
 */
char	*get_env_value(t_env *head, const char *key)
{
	t_env	*curr;

	curr = head;
	while (curr)
	{
		if (ft_strcmp(curr->key, key) == 0)
		{
			return (curr->value);
		}
		curr = curr->next;
	}
	return (NULL);
}

/**
 * attempts to change the working directory.
 * @ token: command tokens.
 * @ new_dir: targuet directory to change to.
 * @ type: shell state.
 * calls chdir to switch to a new directory.
 * On error prints error message and sets return code.
 * On success, calls ft_cd_2 for cleanup and env update. 
 */

void	change_dir(char *new_dir, t_shell *type)
{
	if (chdir(new_dir) != 0)
	{
		free(new_dir);
		new_dir = NULL;
		print_error(E_CD);
		type->r_code = 1;
	}
	else
		ft_cd_2(type, new_dir);
}
