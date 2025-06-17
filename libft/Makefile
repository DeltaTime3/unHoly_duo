# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: afilipe- <afilipe-@student.42porto.com>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/16 12:39:09 by ppaula-d          #+#    #+#              #
#    Updated: 2025/06/17 12:03:52 by afilipe-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#--------------------------------------=---------------------------------------#
#			           VARIABLES				       #
#--------------------------------------=---------------------------------------#

NAME    = libft.a
CC      = cc
CFLAGS  = -Wall -Wextra -Werror
SRC_DIR = libft
SRCS    = ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c ft_isprint.c \
        ft_strlen.c  ft_memset.c ft_bzero.c ft_memcpy.c ft_memmove.c \
        ft_strlcpy.c ft_strlcat.c ft_toupper.c ft_tolower.c ft_strchr.c \
        ft_strrchr.c ft_strncmp.c ft_memchr.c ft_memcmp.c ft_strnstr.c \
        ft_atoi.c ft_calloc.c ft_strdup.c ft_substr.c ft_strjoin.c \
        ft_strtrim.c ft_split.c ft_itoa.c ft_strmapi.c ft_striteri.c \
        ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c \
        ft_free.c ft_isdigitplussign.c ft_isnumeric.c ft_error.c ft_atol.c \
        ft_free_str.c ft_arraylen.c ft_free_gnl.c ft_strcmp.c ft_atoi_base.c \
        ft_ishex.c ft_isspace.c ft_lstadd_back.c ft_lstnew.c ft_printf_fd.c \
		ft_print_hex.c ft_printf_fd.c ft_print_pointer.c ft_print_unsigned.c \
		ft_printc.c ft_printnbr.c ft_printf.c ft_printstr.c ft_put_hex.c \
		ft_uitoa.c ft_strcpy.c ft_strcat.c
        
OBJS    = $(SRCS:.c=.o)
AR      = ar rcs
RM      = rm -f

#--------------------------------------=---------------------------------------#
#			             RULES      			       #
#--------------------------------------=---------------------------------------#

$(NAME): $(OBJS)
	$(AR) $(NAME) $(OBJS)

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

all:    $(NAME)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all fclean re
