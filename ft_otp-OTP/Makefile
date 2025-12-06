CC 			 = cc
FLAGS 		 =	-Wall -Wextra -Werror -MMD -MP -g
INCLUDEPATH  =	./includes/
SRCPATH 	 =	./srcs/
SRCNAME 	 =	main.c 			 		\
			    utils.c 		 		\
				errors.c 				
SRC			 = $(addprefix $(SRCPATH), $(SRCNAME))
NAME 		=	ft_otp
OBJ 		=	$(SRC:.c=.o)
DEP		    = 	$(patsubst %.o,%.d, $(OBJ))

$(NAME): $(OBJ) $(LIB)
	$(CC) $(FLAGS) $(OBJ) -o $(NAME) -lcrypto

%.o: %.c 
	$(CC) $(FLAGS) -I$(INCLUDEPATH) -c $< -o $@

all: $(NAME)

clean:
	rm -f $(OBJ) $(DEP)

fclean: clean
	rm -f $(NAME)

re : fclean all

.PHONY: clean fclean all re

-include $(DEP)