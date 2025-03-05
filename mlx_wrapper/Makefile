CC = gcc
CFLAGS = -Wall -Werror -Wextra

SOURCES = \
c_files/KEYCODE.c c_files/TOOLS.c c_files/INPUT_MOUSE.c c_files/FREE_c.c c_files/TIME2.c \
c_files/INPUT.c c_files/TIME.c c_files/FREE_b.c c_files/IMAGES_b.c \
c_files/INIT_WRAPPER.c c_files/IMAGES_a.c c_files/FREE_a.c c_files/AUDIO.c c_files/COLORS.c \
c_files/VECTOR_2.c c_files/VECTOR_4.c c_files/VECTOR_3.c c_files/VECTOR_3float.c

INCLUDES = -I mlx -I libft
MLXFLAGS = -L mlx -lmlx -framework OpenGL -framework AppKit -lz

LIBFTPATH = libft/
LIBFT = $(LIBFTPATH)libft.a

LIBNAME = mlx_wrapper.a
SAN = -fsanitize=address

OBJ = $(SOURCES:.c=.o)

NAME = mlx_wrapper

all: $(LIBNAME)

$(LIBNAME): $(OBJ) $(LIBFT)
	ar rcs $(LIBNAME) $(OBJ)
	@echo "$(NAME) compiled"

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(LIBFT):
	make -C $(LIBFTPATH)

clean:
	rm -f $(OBJ)
	make -C $(LIBFTPATH) clean

fclean: clean
	rm -f $(LIBNAME)
	rm -f $(LIBFT)
	make -C $(LIBFTPATH) fclean

re: fclean all

.PHONY: all clean fclean re
