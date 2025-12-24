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
C_FLAGS = -Wall -Werror -Wextra -g

SRC_DIR = ./src
LIBFT = $(SRC_DIR)/libft/libft.a

C_FILES = main.c

SRC = $(addprefix $(SRC_DIR)/, $(C_FILES))

OBJ = $(SRC:.c=.o)

INCLUDES = ./headers

#  -- Rules  --
all: $(LIBFT) $(NAME)

$(LIBFT):
	make -C $(SRC_DIR)/libft/

$(NAME): $(OBJ)
	@$(CC) $(C_FLAGS) $(OBJ) $(LIBFT) -o $(NAME)

%.o: %.c
	@echo "Compiling $<..."
	@$(CC) $(C_FLAGS) -c $< -o $@ -I$(INCLUDES)

test:


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

re: fclean all

.PHONY: all clean fclean re
