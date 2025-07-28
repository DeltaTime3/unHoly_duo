
# Compiler and flags
CC				= cc
CFLAGS			= 	-g -Wall -Werror -Wextra

# Directories
FT_BUILTINS_DIR	= ft_builtins
FT_PARSING_DIR	= ft_parsing
INCLUDE_DIR		= ../include
LIBFT_DIR		= ../libft

# Source files
SRC				= env_initializer.c \
				  env_initializer_helpers.c \
				  env_management.c \
				  env_management_utils.c \
				  env_management_utils2.c \
				  env_management_utils3.c \
				  errors.c \
				  exect.c \
				  exect_helpers.c \
				  exect_helpers2.c \
				  exect_helpers3.c \
				  exect_helpers4.c \
				  signal.c \
				  cd_utils.c \
				  cd_utils2.c \
				  export_utils.c \
				  export_utils2.c \
				  export_utils3.c \
				  ft_cd.c \
				  ft_echo.c \
				  ft_env.c \
				  ft_exit.c \
				  ft_exit2.c \
				  ft_export.c \
				  ft_export2.c \
				  ft_pwd.c \
				  ft_unset.c \
				  unset.c \
				  utils.c \
				  expansions.c \
				  expansions_helpers.c \
				  expansions_helpers2.c \
				  expansions_helpers3.c \
				  expansions_helpers4.c \
				  here_doc.c \
				  here_doc_helpers.c \
				  here_doc_helpers2.c \
				  main.c \
				  main_helpers.c \
				  parsing_utils.c \
				  parsing_utils2.c \
				  parsing_utils3.c \
				  parsing_utils4.c \
				  pipes.c \
				  pipes_handlers.c \
				  pipes_handlers2.c \
				  refractors.c \
				  refractors_helpers.c \
				  tokens_helpers.c \
				  tokens_helpers2.c \
				  tokens_helpers3.c \
				  tokens_helpers4.c \
				  tokens_helpers5.c \
				  tokens_helpers6.c \
				  tokens.c \

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

# valgrind:
# 	@echo "{\n   leak readline\n   Memcheck:Leak\n...\n   fun:readline\n}\n{\n   leak add_history\n   Memcheck:Leak\n...\n   fun:add_history\n}" > readline.supp
# 	@valgrind --suppressions=readline.supp --leak-check=full -s --show-leak-kinds=all ./$(NAME)

clean:
	rm -f $(OBJS) $(TESTER_OBJ)
	make -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME) $(TESTER_BIN)
	make -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re tester valgrind
