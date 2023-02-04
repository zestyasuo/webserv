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

SIEGE_DIR	=	siege
SIEGE_BIN	=	$(SIEGE_DIR)/bin/siege
SIEGE_ARGS	=	http://localhost:8080 -b --time=3S
SIEGE_TAR	=	https://download.joedog.org/siege/siege-latest.tar.gz

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
	

$(SIEGE_BIN):
	curl -L -o siege.tar.gz $(SIEGE_TAR)
	mkdir siege_src
	tar -xf siege.tar.gz -C siege_src --strip-components 1
	rm siege.tar.gz
	cd siege_src && ./configure --prefix=$(CURDIR)/$(SIEGE_DIR) --bindir=$(CURDIR)/$(SIEGE_DIR)/bin --mandir=$(CURDIR)/$(SIEGE_DIR)/man
	make -C siege_src
	make install -C siege_src
	rm -rf siege_src

clean:
	rm -rf ${OBJ_PATH}

fclean: clean
	rm -f ${NAME}

s	: $(NAME) $(SIEGE_BIN)
	./$(NAME) &
	sleep 1
	./$(SIEGE_BIN) $(SIEGE_ARGS) &> siege.log
#	$(shell bash -c "./$(NAME) &> /dev/null & ./$(SIEGE_PATH) $(SIEGE_ARGS) > siege.log; pkill webserv")
	pkill webserv
	@echo "\n\n"
	cat siege.log
	rm siege.log

r	: $(NAME)
	./$(NAME)

re : fclean all
