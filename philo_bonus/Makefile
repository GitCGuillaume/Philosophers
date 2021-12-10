SRCS_PATH	=	./srcs

PROCESS_PATH	=	./process

ROUTINES_PATH	=	./routines

INIT_PATH	=	./init

SRCS_FILES	=	$(SRCS_PATH)/ft_args.c $(SRCS_PATH)/ft_atoi.c $(SRCS_PATH)/ft_itoa.c $(SRCS_PATH)/ft_strdup.c $(SRCS_PATH)/ft_str_is_digit.c $(SRCS_PATH)/ft_strjoin.c $(SRCS_PATH)/threads_tools.c $(SRCS_PATH)/tools.c

SRCS_INIT	=	$(INIT_PATH)/initialization.c

SRCS_PROCESS	=	$(PROCESS_PATH)/process.c $(PROCESS_PATH)/process_two.c

SRCS_ROUTINES	=	$(ROUTINES_PATH)/routines.c $(ROUTINES_PATH)/routines_tools.c $(SRCS_PATH)/display.c

SRCS		=	main_bonus.c free.c free_two.c running_process.c

OBJS_FILES	=	$(SRCS_FILES:.c=.o)

OBJS_INIT	=	$(SRCS_INIT:.c=.o)

OBJS_ROUTINES	=	$(SRCS_ROUTINES:.c=.o)

OBJS_PROCESS	=	$(SRCS_PROCESS:.c=.o)

OBJS	=	$(SRCS:.c=.o)

CLANG	=	clang -Wall -Wextra -Werror

RM	=	rm -f

NAME	=	philo_bonus

.c.o:
	$(CLANG) -Iincludes -c $< -o $(<:.c=.o)

$(NAME):	$(OBJS) $(OBJS_FILES) $(OBJS_INIT) $(OBJS_ROUTINES) $(OBJS_PROCESS)
	$(CLANG) -pthread -Iincludes -o $(NAME) $(OBJS) $(OBJS_FILES) $(OBJS_INIT) $(OBJS_ROUTINES) $(OBJS_PROCESS)

all:	$(NAME)

clean:
	$(RM) $(OBJS_FILES)
	$(RM) $(OBJS_INIT)
	$(RM) $(OBJS_PROCESS)
	$(RM) $(OBJS_ROUTINES)
	$(RM) $(OBJS)

fclean:
	$(MAKE) clean
	rm -f $(NAME)

re:
	$(MAKE) fclean
	$(MAKE) all

bonus:
	$(MAKE) re

.PHONY: all clean fclean re

