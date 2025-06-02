# Compiler and flags
CC = cc
CFLAGS = -g -Wall -Werror -Wextra

# Directories
INCLUDE_DIR = ../../include
LIBFT_DIR = ../../libft
SRC_DIR = .
OBJ_DIR = ./obj

# Source files
SRC_FILES = cd_test.c cd_utils.c ft_cd.c cd_utils2.c 
OBJ_FILES = $(addprefix $(OBJ_DIR)/, $(SRC_FILES:.c=.o))

# Tester files
TESTER_SRC = tester_unset.c \
	ft_unset.c \
	export_utils2.c \
	../env_management_utils.c \
	../env_management.c \


TESTER_OBJ = $(addprefix $(OBJ_DIR)/, $(notdir $(TESTER_SRC:.c=.o)))
TESTER_BIN = tester_unset

# Output binary
TARGET = cd_test

# Library
LIBFT = $(LIBFT_DIR)/libft.a

# Rules
all: $(TARGET) tester

$(TARGET): $(OBJ_FILES) $(LIBFT)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -I$(LIBFT_DIR) $(OBJ_FILES) -L$(LIBFT_DIR) -lft -o $(TARGET)

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -I$(LIBFT_DIR) -c $< -o $@

$(OBJ_DIR)/%.o: ../%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -I$(LIBFT_DIR) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(LIBFT):
	make -C $(LIBFT_DIR)

$(TESTER_BIN): $(TESTER_OBJ) $(LIBFT)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -I$(LIBFT_DIR) $(TESTER_OBJ) -L$(LIBFT_DIR) -lft -o $(TESTER_BIN)

tester: $(TESTER_BIN)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(TARGET) $(TESTER_BIN)
	make -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re tester
