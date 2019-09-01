# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lglover <lglover@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/11/26 09:56:17 by lglover           #+#    #+#              #
#    Updated: 2019/08/26 13:19:26 by lglover          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = gcc
CFLAGS = -c -Wall -Wextra -Werror

NAME = libft.a
INCLUDES = ./includes

SRC_PATH = sources
SRC_LIST = ft_atoi.c ft_bzero.c ft_isalnum.c ft_isalpha.c ft_isascii.c ft_isdigit.c\
			ft_isprint.c ft_itoa.c ft_lstadd.c ft_lstdel.c ft_lstdelone.c ft_lstiter.c ft_lstmap.c\
			ft_lstnew.c ft_memalloc.c ft_memccpy.c ft_memchr.c ft_memcmp.c ft_memcpy.c ft_memdel.c\
			ft_memmove.c ft_memset.c ft_putchar.c ft_putchar_fd.c ft_putendl.c ft_putendl_fd.c\
			ft_putnbr.c ft_putnbr_fd.c ft_putstr.c ft_putstr_fd.c ft_strcat.c ft_strchr.c ft_strclr.c\
			ft_strcmp.c ft_strcpy.c ft_strdel.c ft_strdup.c ft_strequ.c ft_striter.c ft_striteri.c\
			ft_strjoin.c ft_strlcat.c ft_strlen.c ft_strmap.c ft_strmapi.c ft_strncat.c ft_strncmp.c\
			ft_strncpy.c ft_strnequ.c ft_strnew.c ft_strnstr.c ft_strrchr.c ft_strsplit.c ft_strstr.c\
			ft_strsub.c ft_strtrim.c ft_tolower.c ft_toupper.c ft_gnl.c \
			ft_delarr.c
SRC = $(addprefix $(SRC_PATH)/, $(SRC_LIST))

OBJ_LIST = $(SRC_LIST:.c=.o)
OBJ_PATH = obj
OBJ = $(addprefix $(OBJ_PATH)/, $(OBJ_LIST))

YELLOW = \033[1;33m
PURPLE = \033[0;35m
NC = \033[0m

.PHONY: all

all: intro make_obj $(NAME)
	@echo "$(PURPLE)MAKE $(NAME) Done!$(NC)"

intro:
	@echo "\n$(PURPLE)MAKE $(NAME) Start!$(NC)"

make_obj :
	@mkdir -p obj

$(NAME): $(OBJ)
	@ar rc $(NAME) $(OBJ)
	@ranlib $(NAME)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c
	@echo "$(YELLOW)$(NAME): $(notdir $<)$(NC)"
	@$(CC) $(CFLAGS) -I $(INCLUDES) -o $@ -c $<

clean :
	@echo "$(YELLOW)Objects Deleted.$(NC)"
	@/bin/rm -rf $(OBJ_PATH)

fclean :	clean
	@echo "$(YELLOW)$(NAME) Deleted.$(NC)"
	@/bin/rm -f $(NAME)

re :	fclean all