UNAME_S := $(shell uname -s)
SAN_FLAGS = address,undefined
ifeq ($(UNAME_S), Linux)
	SAN_FLAGS := $(SAN_FLAGS),leak
endif

CC			=	c++
CFLAGS		=	-Wall -Wextra -Werror -std=c++98 -g -fsanitize=$(SAN_FLAGS)
SRC_PATH	=	src
OBJ_PATH	=	obj
INC_PATH	=	inc

SIEGE_DIR	=	siege
SIEGE_BIN	=	$(SIEGE_DIR)/bin/siege
SIEGE_ARGS	=	http://localhost:8090 -b \
				--time=5S \
				--rc=siegerc \
				--concurrent=50
SIEGE_TAR	=	https://download.joedog.org/siege/siege-latest.tar.gz

SRC			=	$(wildcard ${SRC_PATH}/*.cpp)

HEADERS		=	${wildcard ${INC_PATH}/*.hpp}

HEAD_DEP	=	${HEADERS:${INC_PATH}/%.hpp=${INC_PATH}/%.hpp}

OBJ			=	${SRC:${SRC_PATH}/%.cpp=${OBJ_PATH}/%.o}

NAME		=	 webserv

.PHONY: all fclean clean re test

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
	rm -rf tests/webserv

r	: $(NAME)
	./$(NAME)

s	: $(NAME) $(SIEGE_BIN)
	./$(NAME) test.conf &
	sleep 1
	./$(SIEGE_BIN) $(SIEGE_ARGS) &> siege.log
	pkill webserv
	@echo "\n\n"
	cat siege.log
	rm siege.log

re : fclean all

.PHONY: conf_test

conf_test: $(SRC_PATH)/ConfigStream.cpp
	@${CC} ${CFLAGS} -DCONF_TEST -o $@ $^ -I${INC_PATH}
	@./$@
	@$(RM) $@

