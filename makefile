SRC_UTILS	= \
c_files/init/init_map.c c_files/init/init_labels.c c_files/init/init_ents_data.c c_files/init/init_map_data.c \
c_files/init/init_entities.c c_files/init/init_cube.c c_files/raycasting/draw_sprite_column.c c_files/raycasting/draw_wall_column.c \
c_files/raycasting/rays.c c_files/tools/debug.c c_files/tools/math_tools.c c_files/tools/text.c c_files/tools/image_tools.c \
c_files/tools/ftoa.c c_files/update/update_plr.c c_files/update/update.c c_files/update/movement.c c_files/update/update_ents.c \
c_files/render/render_minimap.c c_files/render/render.c c_files/render/render_background.c c_files/particles/particles.c \
c_files/collisions/collisions_old.c c_files/collisions/collisions.c c_files/raycasting/ray_visualize.c c_files/render/filter.c \
c_files/update/update_menu.c c_files/init/init_menu.c c_files/init/init_hud.c c_files/update/update_projectile.c c_files/update/update_input.c \


SRC			= main.c
GAME_NAME	= cube

GNL_DIR		= ressources/gnl/
GNL			= $(GNL_DIR)get_next_line.a

LISTS_DIR	= ressources/lists/
LISTS		= $(LISTS_DIR)lists.a

LIBFT_DIR	= ressources/mlx_wrapper/libft/
LIBFT		= $(LIBFT_DIR)libft.a

MLX_WRP_DIR = ressources/mlx_wrapper/
MLX_WRAPPER = $(MLX_WRP_DIR)mlx_wrapper.a

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Linux)
	PLATFORM = linux
	MLXFLAGS = -L $(MLX_WRP_DIR)mlx_linux -lmlx -lX11 -lXext -lXcursor -lGL -lz -lm
	CFLAGS += -DIS_LINUX
else
	PLATFORM = mac
	MLXFLAGS = -L $(MLX_WRP_DIR)mlx_mac -lmlx -framework OpenGL -framework AppKit -lz
endif

CC			= gcc
CFLAGS		+= -Wall -Wextra -Werror

$(MLX_WRAPPER):
	make -C $(MLX_WRP_DIR) PLATFORM=$(PLATFORM) --no-print-directory

$(LISTS):
	make -C $(LISTS_DIR) --no-print-directory

$(GNL):
	make -C $(GNL_DIR) --no-print-directory

$(LIBFT):
	make -C $(LIBFT_DIR) --no-print-directory

$(GAME_NAME): $(SRC) $(SRC_UTILS) $(MLX_WRAPPER) $(LISTS) $(GNL) $(LIBFT)
	$(CC) $(CFLAGS) $(SRC) $(SRC_UTILS) $(MLX_WRAPPER) $(MLXFLAGS) $(LISTS) $(GNL) $(LIBFT) -o $(GAME_NAME)
	@echo "$(GAME_NAME) Generated on $(UNAME_S)"

leaks: all
	leaks --atExit -- ./cube square.cub 1 15

all: $(GAME_NAME)

debug: $(GAME_NAME) $(MLX_WRAPPER) $(LISTS) $(GNL) $(LIBFT) $(SRC) $(SRC_UTILS)
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
