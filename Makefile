
NAME		= minishell

SRC 		= srcs/tokens.c \
				srcs/tokens_helpers.c
				
OBJS		= $(SRCS:.c=.o)

LIBFT_DIR	= ./libft
LIBFT		= $(LIBFT_DIR)/libft.a

CC			= cc
CFLAGS		= -Wall -Wextra -Werror -g -I. -Ilibft
RM			= rm -f

all:		$(LIBFT) $(NAME)

$(LIBFT):
				@$(MAKE) -C $(LIBFT_DIR)

$(NAME): 	$(OBJS) $(LIBFT)	
				$(CC) $(CFLAGS) $(SRC) $^ -o $(NAME)

%.o: %.c
				$(CC) $(CFLAGS) -c $< -o $@

clean:
				$(RM) $(OBJS)
				@$(MAKE) fclean -C $(LIBFT_DIR)

fclean:		clean
				$(RM) $(NAME)
				@$(MAKE) fclean -C $(LIBFT_DIR)

re:			fclean all

.PHONY:		all clean fclean re
