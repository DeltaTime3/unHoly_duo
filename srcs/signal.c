#include "../include/minishell.h"

void	ft_signals(void)
{
	signal(SIGINT, handle_sig_int);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGPIPE, handle_sig_pipe);
}

void	handle_sig_int(int sig)
{
	if(sig == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		global_sig = 130;
	}
}

void	handle_sig_heredoc(int sig)
{
	if(sig == SIGINT)
	{
		close(STDIN_FILENO);
		write(STDOUT_FILENO,"\n", 1);
		global_sig = 1;
	}
}
void	handle_sig_pipe(int sig)
{
	(void)sig;
	write(STDERR_FILENO, "Broken pipe\n", 12);
}

void	signal_process(t_shell *shell)
{
	if(global_sig == 130)
	{
		shell->exit_code = 130;
		global_sig = 0;
	}
}

void	clean_exit(char *input)
{
	if(input)
		free(input);
	rl_clear_history();
	exit(0);
}