SRC_DIR	= src/

SRC_FILES	=	main.cpp \
			ft_utils.cpp \
			Config.cpp \
			Server.cpp \
			Location.cpp \
			Cluster.cpp

SRC 		= ${addprefix ${SRC_DIR}, ${SRC_FILES}}

################################################################

OBJ_DIR		= obj/

OBJ		= ${patsubst ${SRC_DIR}%.cpp, ${OBJ_DIR}%.o, ${SRC}}

################################################################

CC		= c++
CFLAGS	= -Werror -Wextra -Wall -std=c++98

################################################################

INCLUDES	= -I inc/

################################################################

RM = rm -rf

NAME	= webServer

################################################################

all: $(NAME)

${OBJ_DIR}%.o: ${SRC_DIR}%.cpp
	mkdir -p ${OBJ_DIR}
	${CC} ${CFLAGS} ${INCLUDES} -c $^ -o $@

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ)

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)
	$(RM) $(OBJ_DIR)

re: fclean all

.PHONY: all re clean fclean