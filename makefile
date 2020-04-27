SRCS = bmpread.c \

OBJS = ${SRCS:.c=.o}

NAME	= libbitmap.a
TEST	= test.out

CC		= clang
CFLAGS	= -Wall -Wextra -Werror



${NAME}: ${OBJS} 
	ar rcs ${NAME} ${OBJS}

test:
	cp ../minilibx/libmlx.dylib ./
	clang -o ${TEST} .test/main.c -Wall -Wextra \
		-L ../mlxpp/ -lmlxpp \
		-L ../minilibx -lmlx \

all: ${NAME} ${TEST}

clean:
	rm -f ${OBJS}
	rm -f *.o
	rm -f *.gch

fclean: clean
	rm -f ${NAME}

re: fclean all

.PHONY: all clean fclean re test
