# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ppaula-d <ppaula-d@student.42porto.com>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/14 09:41:12 by ppaula-d          #+#    #+#              #
#    Updated: 2025/05/27 10:48:41 by ppaula-d         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME		= minishell

SRC 		= srcs/tokens.c \
				srcs/tokens_helpers.c \
				srcs/tokens_helpers2.c \
				srcs/main.c \
				srcs/utils.c \
				srcs/refractors.c \
				srcs/utils2.c
				
OBJS		= $(SRCS:.c=.o)

LIBS		= -lreadline

LIBFT_DIR	= ../../libft
LIBFT		= $(LIBFT_DIR)/libft.a

CC			= cc
CFLAGS		= -Wall -Wextra -Werror -g -I. -I$(LIBFT_DIR)
RM			= rm -f

all:		$(LIBFT) $(NAME)

$(LIBFT):
				@$(MAKE) -C $(LIBFT_DIR)

$(NAME): 	$(OBJS) $(LIBS) $(LIBFT)	
				$(CC) $(CFLAGS) $(SRC) $^ -o $(NAME)

%.o: %.c
				$(CC) $(CFLAGS) -c $< -o $@

valgrind: 
	@echo "{\n   leak readline\n   Memcheck:Leak\n...\n   fun:readline\n}\n{\n   leak add_history\n   Memcheck:Leak\n...\n   fun:add_history\n}" > readline.supp
	@valgrind --suppressions=readline.supp --leak-check=full -s --show-leak-kinds=all ./$(NAME)

clean:
				$(RM) $(OBJS)
				@$(MAKE) fclean -C $(LIBFT_DIR)

fclean:		clean
				$(RM) $(NAME)
				@$(MAKE) fclean -C $(LIBFT_DIR)

re:			fclean all

.PHONY:		all clean fclean re
