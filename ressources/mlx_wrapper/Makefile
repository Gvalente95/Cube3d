UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Linux)
    MLX_DIR = mlx_linux
    MLX_LIBS = -lX11 -lXext -lXcursor -lGL -lz -lm
	CFLAGS += -DIS_LINUX
else
    MLX_DIR = mlx_mac
    MLX_LIBS = -framework OpenGL -framework AppKit -lz
    CFLAGS += -DMAC
endif

SOURCES = \
c_files/string_3.c c_files/input_tools.c c_files/vector_2.c c_files/tools.c c_files/input_mouse.c c_files/vectors_show.c \
c_files/vector_3floatb.c c_files/vector_math.c c_files/draw_img.c c_files/vector_3float.c c_files/draw_img_2.c \
c_files/string.c c_files/time2.c c_files/images_data.c c_files/images_load.c c_files/input.c c_files/draw_alpha.c \
c_files/time.c c_files/free.c c_files/free_b.c c_files/vector_4.c c_files/images_b.c c_files/draw_pixels.c \
c_files/init_wrapper.c c_files/Vector_3b.c c_files/vector4float.c c_files/string_2.c c_files/images_a.c \
c_files/free_a.c c_files/vector_2b.c c_files/audio.c c_files/colors.c c_files/vector_3.c c_files/vector_2f_b.c \
c_files/vector_2f.c

OBJ = $(SOURCES:.c=.o)

LIBNAME = mlx_wrapper.a

INCLUDES = -I mlx_wrapper/$(MLX_DIR) -I libft

MLXFLAGS = -L mlx_wrapper/$(MLX_DIR) $(MLX_LIBS)

all: $(LIBNAME)

$(LIBNAME): $(OBJ)
	ar rcs $(LIBNAME) $(OBJ)
	@echo "$(LIBNAME) compiled"

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(LIBNAME)

re: fclean all

.PHONY: all clean fclean re
.SILENT:
