NAME = cub3d
SRC_DIR = srcs
SRCS = main.c
OBJ_DIR = objs
OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))
INC_DIR := incs
INCS = cub3d.h
LIBFT = -Ilibft -Llibft -lft
LIBFT_DIR = libft
CC = cc
CFLAGS = -Wall -Wextra -Werror
INC_FLAGS = -I $(INC_DIR) -Ilibft
CLIB	= -Lmlx -lmlx -framework OpenGL -framework Appkit

all : $(NAME)

$(NAME) : $(OBJS)
	make -sC $(LIBFT_DIR) all
	$(CC) $(CFLAGS) -o $@ $^ $(LIBFT) $(CLIB)

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c -o $@ $< $(INC_FLAGS) -Imlx

clean :
	make -sC $(LIBFT_DIR) fclean
	rm -rf $(OBJ_DIR)

fclean : clean
	rm -rf $(NAME)

re : fclean all

.PHONY : all clean fclean re
