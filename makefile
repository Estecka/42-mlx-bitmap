SRCS = bmpread.c \

OBJS = ${SRCS:.c=.o}

NAME	= libbitmap.a
TEST	= test.out

OS		= $(shell uname)

CC		= clang
CFLAGS	= -Wall -Wextra -Werror
LIBFLAGS = \
		-L ../mlxpp/ -lmlxpp \
		-L ./ -L ../minilibx -lmlx \

ifeq (${OS}, Linux)
LIBFLAGS += \
		-lbsd -lX11 -lXext \

endif



${NAME}: ${OBJS} 
	ar rcs ${NAME} ${OBJS}

test: ${TEST}
${TEST}:
	make -C ../minilibx
ifeq (${OS}, Darwin)
	cp ../minilibx/libmlx.dylib ./
endif
	clang -o ${TEST} .test/main.c -Wall -Wextra \
		${LIBFLAGS} \


all: ${NAME} ${TEST}

clean:
	rm -f ${OBJS}
	rm -f *.o
	rm -f *.gch

fclean: clean
	rm -f ${NAME}
	rm -f ${TEST}

re: fclean all

.PHONY: all clean fclean re test
