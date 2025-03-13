UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Linux)
    MLX_DIR = mlx_linux
    MLX_LIBS = -lX11 -lXext -lXcursor -lGL -lz -lm
	CFLAGS += -DIS_LINUX
else
    MLX_DIR = mlx
    MLX_LIBS = -framework OpenGL -framework AppKit -lz
    CFLAGS += -DMAC
endif

SOURCES = \
c_files/TOOLS.c c_files/INPUT_MOUSE.c c_files/FREE_c.c c_files/TIME2.c \
c_files/INPUT.c c_files/TIME.c c_files/FREE_b.c c_files/IMAGES_b.c \
c_files/INIT_WRAPPER.c c_files/IMAGES_a.c c_files/FREE_a.c c_files/AUDIO.c c_files/COLORS.c \
c_files/VECTOR_2.c c_files/VECTOR_4.c c_files/VECTOR_3.c c_files/VECTOR_3float.c

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
