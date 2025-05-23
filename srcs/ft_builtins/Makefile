# Compiler and flags
CC = cc
CFLAGS = -g -Wall -Werror -Wextra

# Directories
INCLUDE_DIR = ../../include
LIBFT_DIR = ../../libft
SRC_DIR = .
OBJ_DIR = ./obj

# Source files
SRC_FILES = cd_test.c cd_utils.c ft_cd.c
OBJ_FILES = $(addprefix $(OBJ_DIR)/, $(SRC_FILES:.c=.o))

# Output binary
TARGET = cd_test

# Library
LIBFT = $(LIBFT_DIR)/libft.a

# Rules
all: $(TARGET)

$(TARGET): $(OBJ_FILES) $(LIBFT)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -I$(LIBFT_DIR) $(OBJ_FILES) -L$(LIBFT_DIR) -lft -o $(TARGET)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -I$(LIBFT_DIR) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(LIBFT):
	make -C $(LIBFT_DIR)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(TARGET)
	make -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re