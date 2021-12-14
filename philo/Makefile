SRCS	=	srcs

SRCS_PATH	=	$(SRCS)/ft_args.c $(SRCS)/ft_atoi.c $(SRCS)/ft_str_is_digit.c $(SRCS)/threads_tools.c $(SRCS)/tools.c

SRCS_PATH_ONE	=	free.c init/initialization.c init/initialization_two.c routines.c routines_secondary.c running_thread.c running_routine.c running_routine_secondary.c main.c

OBJS	=	$(SRCS_PATH:.c=.o)

OBJS_ONE	=	$(SRCS_PATH_ONE:.c=.o)

CLANG	=	cc -Wall -Wextra -Werror

RM	=	rm -f

NAME	=	philo

.c.o:
	$(CLANG) -Iincludes -c $< -o $(<:.c=.o)

$(NAME):	$(OBJS) $(OBJS_ONE)
	$(CLANG) -Iincludes -pthread -o $(NAME) $(OBJS_ONE) $(OBJS)

all:	$(NAME)

clean:
	$(RM) $(OBJS)
	$(RM) $(OBJS_ONE)

fclean:
	$(MAKE) clean
	$(RM) $(NAME)

re:
	$(MAKE) fclean
	$(MAKE) all

.PHONY: all clean fclean re bonus
