NAME = pipex

NAME_BONUS = pipex_bonus

CC = cc

OBJ_DIR = objects/

OBJ_DIR_BONUS = objects_bonus/

CFLAGS = -Wall -Wextra -Werror -Ilibft

HEADERS = pipex.h libft/libft.h

SOURCES = main.c

# BONUS_SOURCES = main_bonus.c

OBJECTS = $(SOURCES:%.c=$(OBJ_DIR)%.o)

OBJECTS_BONUS = $(BONUS_SOURCES:%.c=$(OBJ_DIR_BONUS)%.o)

LIBFT = libft/libft.a

all: $(NAME) #$(NAME_BONUS)

$(NAME): $(LIBFT) $(OBJECTS)
	$(CC) $(OBJECTS) $(LIBFT) -o $(NAME)

$(LIBFT):
	make -C libft

$(OBJ_DIR)%.o: %.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -o $@ -c $<

$(OBJ_DIR):
	mkdir -p $@

bonus: $(NAME_BONUS)

$(NAME_BONUS): $(LIBFT) $(OBJECTS_BONUS)
	$(CC) $(OBJECTS_BONUS) $(LIBFT) -o $(NAME_BONUS)

$(OBJ_DIR_BONUS)%.o: %.c | $(OBJ_DIR_BONUS)
	$(CC) $(CFLAGS) -o $@ -c $<

$(OBJ_DIR_BONUS):
	mkdir -p $@

clean:
	rm -rf $(OBJ_DIR) $(OBJ_DIR_BONUS)
	make clean -C libft

fclean:
	rm -rf $(OBJ_DIR) $(OBJ_DIR_BONUS) $(NAME) $(NAME_BONUS)
	make fclean -C libft

re: fclean all

.PHONY: all clean fclean re