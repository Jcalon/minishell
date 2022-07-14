# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/07/04 15:18:41 by jcalon            #+#    #+#              #
#    Updated: 2022/07/14 13:52:55 by jcalon           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME	= minishell

SRCS 	=	srcs/main.c	\
			srcs/parsing/parsing.c \
			srcs/parsing/init.c \
			srcs/parsing/syntax_error.c \
			srcs/exec/builtin.c \
			srcs/exec/builtin_echo.c \
			srcs/exec/builtin_env.c \
			srcs/exec/builtin_export.c \
			srcs/exec/builtin_unset.c \
			srcs/exec/exec.c \
			srcs/utils.c \
			srcs/signal.c \
			srcs/history.c \
			srcs/exec/pipe.c	\
			srcs/exec/children.c \
			srcs/error.c \
			srcs/exec/env.c \
			srcs/parsing/var_env.c \
			srcs/parsing/ft_split_minishell.c \
			srcs/parsing/clear_quote.c

OBJS 	= ${SRCS:.c=.o}

CC 		= gcc
CFLAGS 	= -Wall -Wextra -Werror -g

INCLUDES = -I libft -I includes

.c.o:
		@${CC} ${CFLAGS} ${INCLUDES} -c -o $@ $<

all: 	${NAME}

${NAME}: ${OBJS}
		@echo "----Compiling lib----"
		@make re -C libft --no-print-directory
		@$(CC) $(CFLAGS) ${OBJS} -Llibft -lft -lreadline ${INCLUDES} -o ${NAME}
		@echo "\nminishell Compiled!\n"

clean:
		@make clean -C libft --no-print-directory
		@rm -f ${OBJS}

fclean: clean
		@make fclean -C libft --no-print-directory
		@rm -f $(NAME)
		@echo "\nDeleting EVERYTHING!\n"

re:		fclean all

.PHONY: all clean fclean re