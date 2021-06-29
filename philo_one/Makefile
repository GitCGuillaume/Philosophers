SRCS	=	../srcs

SRCS_PATH	=	$(SRCS)/ft_args.c $(SRCS)/ft_atoi.c $(SRCS)/ft_str_is_digit.c

SRCS_PATH_ONE	=	main.c

OBJS	=	$(SRCS_PATH:.c=.o)

OBJS_ONE	=	$(SRCS_PATH_ONE:.c=.o)

CLANG	=	clang -Wall -Wextra

RM	=	rm -f

NAME	=	philo_one

.c.o:
	$(CLANG) -I../includes -c $< -o $(<:.c=.o)

$(NAME):	$(OBJS) $(OBJS_ONE)
	$(CLANG) -I../includes -pthread -o $(NAME) $(OBJS_ONE) $(OBJS)

all:
	$(NAME)

clean:
	$(RM) $(OBJS)
	$(RM) $(OBJS_ONE)
fclean:
	$(MAKE) clean
	$(RM) $(NAME)

re:
	$(MAKE) fclean
	$(MAKE) all

.PHONY: all clean fclean re
