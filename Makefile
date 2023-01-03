CC			=	clang++
CFLAGS		=	-Wall -Wextra -Werror -std=c++98 -g #-fsanitize=address 
SRC_PATH	=	src
OBJ_PATH	=	obj
INC_PATH	=	inc

SRC			=	$(wildcard ${SRC_PATH}/*.cpp)

HEADERS		=	${wildcard ${INC_PATH}/*.hpp}

HEAD_DEP	=	${HEADERS:${INC_PATH}/%.hpp=${INC_PATH}/%.hpp}

OBJ			=	${SRC:${SRC_PATH}/%.cpp=${OBJ_PATH}/%.o}

NAME		=	 webserv

all: ${NAME}

${OBJ_PATH} : 
	mkdir ${OBJ_PATH}
	echo ${HEAD_DEP}

${OBJ_PATH}/%.o : ${SRC_PATH}/%.cpp ${HEAD_DEP} | ${OBJ_PATH}
	${CC} ${CFLAGS} -o $@ -c $< -I${INC_PATH}

${NAME} : ${OBJ}
	${CC} ${CFLAGS} $^ -o $@

clean:
	rm -rf ${OBJ_PATH}

fclean: clean
	rm -f ${NAME}

re : fclean all
