# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/07/04 15:18:41 by jcalon            #+#    #+#              #
#    Updated: 2022/07/06 18:09:39 by jcalon           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME	= minishell

SRCS 	=	srcs/main.c	\
			srcs/parsing/parsing.c \
			srcs/parsing/init.c \
			srcs/parsing/syntax_error.c \
			srcs/builtin.c \
			srcs/exec.c \
			srcs/utils.c

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
		@$(CC) $(CFLAGS) ${OBJS} -Llibft -lft ${INCLUDES} -o ${NAME}
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