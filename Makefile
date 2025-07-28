
# Compiler and flags
CC				= cc
CFLAGS			= 	-g -Wall -Werror -Wextra

# Directories
SRCS_DIR		= srcs
FT_PARSING_DIR	= ft_parsing
INCLUDE_DIR		= include
LIBFT_DIR		= libft

# Source files
SRC				= $(SRCS_DIR)/env_initializer.c \
				  $(SRCS_DIR)/env_initializer_helpers.c \
				  $(SRCS_DIR)/env_management.c \
				  $(SRCS_DIR)/env_management_utils.c \
				  $(SRCS_DIR)/env_management_utils2.c \
				  $(SRCS_DIR)/env_management_utils3.c \
				  $(SRCS_DIR)/errors.c \
				  $(SRCS_DIR)/exect.c \
				  $(SRCS_DIR)/exect_helpers.c \
				  $(SRCS_DIR)/exect_helpers2.c \
				  $(SRCS_DIR)/exect_helpers3.c \
				  $(SRCS_DIR)/exect_helpers4.c \
				  $(SRCS_DIR)/signal.c \
				  $(SRCS_DIR)/cd_utils.c \
				  $(SRCS_DIR)/cd_utils2.c \
				  $(SRCS_DIR)/export_utils.c \
				  $(SRCS_DIR)/export_utils2.c \
				  $(SRCS_DIR)/export_utils3.c \
				  $(SRCS_DIR)/ft_cd.c \
				  $(SRCS_DIR)/ft_echo.c \
				  $(SRCS_DIR)/ft_env.c \
				  $(SRCS_DIR)/ft_exit.c \
				  $(SRCS_DIR)/ft_exit2.c \
				  $(SRCS_DIR)/ft_export.c \
				  $(SRCS_DIR)/ft_export2.c \
				  $(SRCS_DIR)/ft_pwd.c \
				  $(SRCS_DIR)/ft_unset.c \
				  $(SRCS_DIR)/unset.c \
				  $(SRCS_DIR)/utils.c \
				  $(SRCS_DIR)/expansions.c \
				  $(SRCS_DIR)/expansions_helpers.c \
				  $(SRCS_DIR)/expansions_helpers2.c \
				  $(SRCS_DIR)/expansions_helpers3.c \
				  $(SRCS_DIR)/expansions_helpers4.c \
				  $(SRCS_DIR)/here_doc.c \
				  $(SRCS_DIR)/here_doc_helpers.c \
				  $(SRCS_DIR)/here_doc_helpers2.c \
				  $(SRCS_DIR)/here_doc_helpers3.c \
				  $(SRCS_DIR)/main.c \
				  $(SRCS_DIR)/main_helpers.c \
				  $(SRCS_DIR)/parsing_utils.c \
				  $(SRCS_DIR)/parsing_utils2.c \
				  $(SRCS_DIR)/parsing_utils3.c \
				  $(SRCS_DIR)/parsing_utils4.c \
				  $(SRCS_DIR)/pipes.c \
				  $(SRCS_DIR)/pipes_handlers.c \
				  $(SRCS_DIR)/pipes_handlers2.c \
				  $(SRCS_DIR)/refractors.c \
				  $(SRCS_DIR)/refractors_helpers.c \
				  $(SRCS_DIR)/tokens_helpers.c \
				  $(SRCS_DIR)/tokens_helpers2.c \
				  $(SRCS_DIR)/tokens_helpers3.c \
				  $(SRCS_DIR)/tokens_helpers4.c \
				  $(SRCS_DIR)/tokens_helpers5.c \
				  $(SRCS_DIR)/tokens_helpers6.c \
				  $(SRCS_DIR)/tokens.c \

OBJS			= $(SRC:.c=.o)

# Tester files
TESTER_SRC = tester_unset.c \
	ft_unset.c \
	export_utils2.c \
	../env_management_utils.c \
	../env_management.c \
	export_utils.c

TESTER_OBJ = $(TESTER_SRC:.c=.o)
TESTER_BIN = tester_unset

# Output binary
NAME = minishell
LIBFT = $(LIBFT_DIR)/libft.a

# Rules
all: $(LIBFT) $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -I$(LIBFT_DIR) $(OBJS) -L$(LIBFT_DIR) -lft -lreadline -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -I$(LIBFT_DIR) -c $< -o $@

$(LIBFT):
	make -C $(LIBFT_DIR)

tester: $(TESTER_BIN)

$(TESTER_BIN): $(TESTER_OBJ) $(LIBFT)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -I$(LIBFT_DIR) $(TESTER_OBJ) -L$(LIBFT_DIR) -lft -o $(TESTER_BIN)

valgrind:
	@echo "{\n   leak readline\n   Memcheck:Leak\n...\n   fun:readline\n}\n{\n   leak add_history\n   Memcheck:Leak\n...\n   fun:add_history\n}" > readline.supp
	@valgrind --suppressions=readline.supp --leak-check=full -s --show-leak-kinds=all ./$(NAME)

clean:
	rm -f $(OBJS) $(TESTER_OBJ)
	make -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME) $(TESTER_BIN)
	make -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re tester valgrind
