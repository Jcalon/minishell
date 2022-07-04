# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jcalon <jcalon@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/07/04 15:18:41 by jcalon            #+#    #+#              #
#    Updated: 2022/07/04 17:30:03 by jcalon           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME	= minishell

SRCS 	= srcs/main.c

OBJS 	= ${SRCS:.c=.o}

CC 		= gcc
CFLAGS 	= -Wall -Wextra -Werror -g

INCLUDES = -I libft -I includes

.c.o:
		@${CC} ${CFLAGS} ${INCLUDES} -c -o $@ $<

all: 	${NAME}

${NAME}: ${OBJS}
		@echo "\033[33m----Compiling lib----"
		@make re -C libft --no-print-directory
		@$(CC) $(CFLAGS) ${OBJS} -Llibft -lft ${INCLUDES} -o ${NAME}
		@echo "\033[32mminishell Compiled! ᕦ(\033[31m♥\033[32m_\033[31m♥\033[32m)ᕤ\n"

clean:
		@make clean -C libft --no-print-directory
		@rm -f ${OBJS}

fclean: clean
		@make fclean -C libft --no-print-directory
		@rm -f $(NAME)
		@echo "\n\033[31mDeleting EVERYTHING! ⌐(ಠ۾ಠ)¬\n"

re:		fclean all

.PHONY: all clean fclean re