
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
				  env_management.c \
				  env_management_utils.c \
				  errors.c \
				  exect.c \
				  signal.c \
				  $(FT_BUILTINS_DIR)/cd_utils.c \
				  $(FT_BUILTINS_DIR)/cd_utils2.c \
				  $(FT_BUILTINS_DIR)/export_utils.c \
				  $(FT_BUILTINS_DIR)/export_utils2.c \
				  $(FT_BUILTINS_DIR)/export_utils3.c \
				  $(FT_BUILTINS_DIR)/ft_cd.c \
				  $(FT_BUILTINS_DIR)/ft_echo.c \
				  $(FT_BUILTINS_DIR)/ft_env.c \
				  $(FT_BUILTINS_DIR)/ft_exit.c \
				  $(FT_BUILTINS_DIR)/ft_exit2.c \
				  $(FT_BUILTINS_DIR)/ft_export.c \
				  $(FT_BUILTINS_DIR)/ft_export2.c \
				  $(FT_BUILTINS_DIR)/ft_pwd.c \
				  $(FT_BUILTINS_DIR)/ft_unset.c \
				  $(FT_BUILTINS_DIR)/unset.c \
				  $(FT_BUILTINS_DIR)/utils.c \
				  $(FT_PARSING_DIR)/expansions.c \
				  $(FT_PARSING_DIR)/here_doc.c \
				  $(FT_PARSING_DIR)/main.c \
				  $(FT_PARSING_DIR)/parsing_utils.c \
				  $(FT_PARSING_DIR)/parsing_utils2.c \
				  $(FT_PARSING_DIR)/pipes.c \
				  $(FT_PARSING_DIR)/refractors.c \
				  $(FT_PARSING_DIR)/tokens_helpers.c \
				  $(FT_PARSING_DIR)/tokens_helpers2.c \
				  $(FT_PARSING_DIR)/tokens.c \

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
