# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kmonjard <kmonjard@student.42berlin.d      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/12/19 11:58:57 by kmonjard          #+#    #+#              #
#    Updated: 2025/12/19 11:59:58 by kmonjard         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = gcc
C_FLAGS = -g

SRC_DIR = ./src
LIBFT = $(SRC_DIR)/libft/libft.a

C_FILES = tmp/main.c main/prompt.c main/initializer.c env/environment.c \
	input/signals.c main/raw_mode.c input/history.c \
	input/arrows.c input/input.c input/input_utils.c lexer/lexer.c\
	execution/execute.c execution/run_child.c execution/commands.c \
	execution/path_resolver.c utils/cleanup.c execution/metacharacters.c \
	utils/free_shell.c lexer/token.c execution/commands_utils.c \
	input/write_and_del.c main/initializer_utils.c execution/check_syntax.c \
	execution/expand.c builtins/ft_echo.c builtins/ft_cd.c builtins/ft_env.c \
	builtins/ft_exit.c builtins/ft_export.c builtins/ft_pwd.c \
	builtins/ft_unset.c execution/heredoc.c execution/heredoc_utils.c \
	redir/redirs.c

SRC = $(addprefix $(SRC_DIR)/, $(C_FILES))

C_OBJS = $(C_FILES:.c=.o)

OBJ_DIR = bin
OBJ = $(addprefix $(OBJ_DIR)/, $(C_OBJS))

INCLUDES = ./headers

#  -- Rules  --
all: $(LIBFT) $(NAME)

$(LIBFT):
	make -C $(SRC_DIR)/libft/

$(NAME): $(OBJ)
	@$(CC) $(C_FLAGS) $(OBJ) $(LIBFT) -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo "Compiling $<..."
	@$(CC) $(C_FLAGS) -c $< -o $@ -I$(INCLUDES)

clean:
	@make -C ./src/libft clean
	@echo "Deleting object files in minishell..."
	@rm -f $(OBJ) $(O_BONUS)

fclean:
	@make -C ./src/libft fclean
	@echo "Deleting object files in minishell..."
	@echo "Deleting all binaries in minishell..."
	@rm -f $(NAME)
	@rm -f $(OBJ) $(O_BONUS)
	@rmdir bin/*
	@rmdir bin

re: fclean all

.PHONY: all clean fclean re
