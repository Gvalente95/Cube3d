SRC_UTILS	= \
c_files/init/init_map.c c_files/init/init_cube.c c_files/tools/free.c c_files/update/update.c \
c_files/parsing/checker.c c_files/parsing/string_tools.c c_files/parsing/floodfill.c c_files/parsing/strcmp_tools.c \
c_files/render/render.c c_files/init/init_map_data.c c_files/tools/debug.c c_files/tools/text.c \
c_files/tools/math_tools.c c_files/tools/image_tools.c

SRC			= main.c
GAME_NAME	= cube

GNL_DIR		= gnl/
GNL			= $(GNL_DIR)get_next_line.a

LISTS_DIR	= lists/
LISTS		= lists/lists.a

LIBFT_DIR	= mlx_wrapper/libft/
LIBFT		= $(LIBFT_DIR)libft.a

MLX_WRP_DIR = mlx_wrapper/
MLX_WRAPPER = $(MLX_WRP_DIR)mlx_wrapper.a
MLXFLAGS = -L mlx_wrapper/mlx -lmlx -framework OpenGL -framework AppKit -lz

CC			= gcc
CFLAGS		= -Wall -Wextra -Wall

$(MLX_WRAPPER):
	make -C $(MLX_WRP_DIR) --no-print-directory

$(LISTS):
	make -C $(LISTS_DIR) --no-print-directory

$(GNL):
	make -C $(GNL_DIR) --no-print-directory

$(LIBFT):
	make -C $(LIBFT_DIR) --no-print-directory

$(GAME_NAME): $(MLX_WRAPPER) $(LISTS) $(GNL) $(LIBFT)
	$(CC) $(CFLAGS) $(SRC) $(SRC_UTILS) $(MLX_WRAPPER) $(MLXFLAGS) $(LISTS) $(GNL) $(LIBFT) -o $(GAME_NAME)
	@echo "$(GAME_NAME) Generated"

all: $(GAME_NAME)

debug: $(GAME_NAME) $(MLX_WRAPPER) $(LISTS) $(GNL) $(LIBFT)
	$(CC) -fsanitize=address -g $(CFLAGS) $(SRC) $(SRC_UTILS) $(MLX_WRAPPER) $(MLXFLAGS) $(LISTS) $(GNL) $(LIBFT) -o $(GAME_NAME)
	@echo "$(GAME_NAME) debug Generated"

clean:
	make -C $(LIBFT_DIR) --no-print-directory clean
	make -C $(MLX_WRP_DIR) --no-print-directory clean
	make -C $(LISTS_DIR) --no-print-directory clean
	make -C $(GNL_DIR) --no-print-directory clean
	rm -f $(GAME_NAME)
	@echo "$(GAME_NAME) $(CYAN)cleaned$(RESET)"

quiet_clean:
	make -C $(LIBFT_DIR) --no-print-directory clean QUIET=1
	make -C $(MLX_WRP_DIR) --no-print-directory clean QUIET=1
	make -C $(LISTS_DIR) --no-print-directory clean QUIET=1
	make -C $(GNL_DIR) --no-print-directory clean QUIET=1
	rm -f $(GAME_NAME)

fclean: quiet_clean
	make -C $(LIBFT_DIR) --no-print-directory fclean QUIET=1
	make -C $(MLX_WRP_DIR) --no-print-directory fclean QUIET=1
	make -C $(LISTS_DIR) --no-print-directory fclean QUIET=1
	make -C $(GNL_DIR) --no-print-directory fclean QUIET=1
	@echo "$(GAME_NAME) $(CYAN)force cleaned$(RESET)"

re: fclean all

phony: all re bonus clean fclean
.SILENT: