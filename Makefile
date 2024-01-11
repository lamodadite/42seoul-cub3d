NAME = cub3D
SRC_DIR = srcs
SRCS = cu_parse_texture.c cu_parse_map.c cu_utils.c cu_debug.c \
cu_init.c cub3d.c cu_load_data.c cu_check_key.c cu_cast_floor.c cu_cast_wall.c cu_draw_minimap.c
INCS = cub.h keys.h
OBJ_DIR = objs
OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))
INC_DIR = incs
INCS = cub.h keys.h
LIBFT = -Ilibft -Llibft -lft
LIBFT_DIR = libft
CC = cc
CFLAGS = -Wall -Wextra -Werror -g -fsanitize=address
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
