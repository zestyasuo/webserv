UNAME_S := $(shell uname -s)
SAN_FLAGS = address,undefined
ifeq ($(UNAME_S), Linux)
	SAN_FLAGS := $(SAN_FLAGS),leak
endif
# ifeq ($(UNAME_S), Darwin)
	
# endif

CC			=	c++
CFLAGS		=	-Wall -Wextra -Werror -std=c++98 -g -fsanitize=$(SAN_FLAGS)
SRC_PATH	=	src
OBJ_PATH	=	obj
INC_PATH	=	inc

SIEGE_PATH	=	siege/osx/bin/siege
SIEGE_ARGS	=	http://localhost:8080 -b --time=3S

SRC			=	$(wildcard ${SRC_PATH}/*.cpp)

HEADERS		=	${wildcard ${INC_PATH}/*.hpp}

HEAD_DEP	=	${HEADERS:${INC_PATH}/%.hpp=${INC_PATH}/%.hpp}

OBJ			=	${SRC:${SRC_PATH}/%.cpp=${OBJ_PATH}/%.o}

NAME		=	 webserv

.PHONY: all fclean clean re test r

all: ${NAME}

test: all
	cd tests ; bash test_serv.sh

${OBJ_PATH} : 
	mkdir ${OBJ_PATH}

${OBJ_PATH}/%.o : ${SRC_PATH}/%.cpp ${HEAD_DEP} | ${OBJ_PATH}
	${CC} ${CFLAGS} -o $@ -c $< -I${INC_PATH}

${NAME} : ${OBJ}
	${CC} ${CFLAGS} $^ -o $@

clean:
	rm -rf ${OBJ_PATH}

fclean: clean
	rm -f ${NAME}

s	: $(NAME)
	./$(NAME) &
	sleep 1
	./$(SIEGE_PATH) $(SIEGE_ARGS) &> siege.log
#	$(shell bash -c "./$(NAME) &> /dev/null & ./$(SIEGE_PATH) $(SIEGE_ARGS) > siege.log; pkill webserv")
	pkill webserv
	@echo "\n\n"
	cat siege.log
	rm siege.log

r	: $(NAME)
	./$(NAME)

re : fclean all
