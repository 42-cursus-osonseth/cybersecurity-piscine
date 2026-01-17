CC 			 = cc
FLAGS 		 = -Wall -Wextra -Werror -MMD -MP -g
LDFLAGS      = -lpcap
INCLUDEPATH  = ./includes/
SRCPATH 	 = ./src/
SRCNAME 	 = main.c print_func.c parsing.c buffer_build.c raw_socket.c signal_handle.c
SRC			 = $(addprefix $(SRCPATH), $(SRCNAME))
NAME 		 = Inquisitor
OBJ 		 = $(SRC:.c=.o)
DEP		     = $(patsubst %.o,%.d, $(OBJ))

$(NAME): $(OBJ)
	$(CC) $(FLAGS) $(OBJ) -o $(NAME) $(LDFLAGS)

%.o: %.c 
	$(CC) $(FLAGS) -I$(INCLUDEPATH) -c $< -o $@

all: $(NAME)

clean:
	rm -f $(OBJ) $(DEP)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: clean fclean all re

-include $(DEP)
