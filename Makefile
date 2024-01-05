NAME = cub3d
SRC_DIR = srcs
SRCS = cub3d.c cu_load_data.c cu_check_key.c cu_cast_floor.c cu_cast_wall.c
INCS = cub.h keys.h
#SRCS = main.c
#INCS = cub3d.h keys.h
OBJ_DIR = objs
OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))
INC_DIR := incs
LIBFT = -Ilibft -Llibft -lft
LIBFT_DIR = libft
CC = cc
CFLAGS = -Wall -Wextra -Werror -fsanitize=address -g
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
