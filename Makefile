# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lglover <lglover@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/11/26 09:56:17 by lglover           #+#    #+#              #
#    Updated: 2020/03/23 17:05:14 by lglover          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = gcc
CFLAGS = -Wall -Wextra -Werror

NAME = doom-nukem
INCLUDES = 	-I ./libft/includes \
			-I ./sdl2/SDL2.framework/Headers \
			-I ./sdl2/SDL2_mixer.framework/Headers \
			-I ./sdl2/SDL2_ttf.framework/Headers \
			-I ./includes

SRC_PATH =	sources
SRC_LIST =	actions.c \
            app.c \
            bmp_reader.c \
            clipping.c \
            collision.c \
            decor.c \
            draw.c \
            draw_line.c \
            edge.c \
            editor.c \
            editor_inputs.c \
            errors.c \
            event_handling.c \
            events.c \
            floor_collision.c \
            font.c \
            fpc_counter.c \
            game.c \
            game_data.c \
            gradient.c \
            grid.c \
            hit_fill.c \
            hud_image.c \
            init.c \
            init_sdl.c \
            intersect.c \
            live_mode_add.c \
            live_mode_inputs.c \
            live_mode_sector.c \
            live_mode_set.c \
            live_mode_toggle.c \
            live_mode_utils.c \
            live_mode_wall.c \
            loaders.c \
            main.c \
            map_markers.c \
            matrix_handling.c \
            matrix_utils.c \
            pipeline.c \
            pixel.c \
            player_utils.c \
            points.c \
            points_mode_inputs.c \
            polygon_list.c \
            ray.c \
            render_map.c \
            render_sector.c \
            reset_screen.c \
            room_polygon.c \
            room_polygon_utils.c \
            scanline_threads.c \
            scanline_triangle.c \
            sector.c \
            sector_close.c \
            sector_close_utils.c \
            skybox.c \
            switch_mode.c \
            triangle.c \
            triangulation.c \
            triangulation_utils.c \
            ui_draw.c \
            utils.c \
            vector.c \
            vector_handling.c \
            vertex_list.c \
            view_matrix.c \
            wall.c \
            wall_inside_outside.c \

SRC = $(addprefix $(SRC_PATH)/, $(SRC_LIST))

OBJ_LIST = $(SRC_LIST:.c=.o)
OBJ_PATH = obj
OBJ = $(addprefix $(OBJ_PATH)/, $(OBJ_LIST))

LIBFT_PATH = libft
LIBFT = -L $(LIBFT_PATH) -lft

LIBS = 	-lm \
		-lpthread \
		-F ./sdl2 -framework SDL2 -framework SDL2_mixer -framework SDL2_ttf

YELLOW = \033[1;33m
PURPLE = \033[0;35m
NC = \033[0m

.PHONY: all

all: make_libft intro make_obj $(NAME)
	@echo "$(PURPLE)MAKE $(NAME) Done!$(NC)"

intro:
	@echo "\n$(PURPLE)MAKE $(NAME) Start!$(NC)"

$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) $(INCLUDES) $(LIBS) $(OBJ) -o $(NAME) $(LIBFT)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c
	@echo "$(YELLOW)$(NAME): $(notdir $<)$(NC)"
	@$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

make_obj :
	@mkdir -p obj

make_libft :
	@make -C ./libft/

clean :
	@echo "$(YELLOW)Objects Deleted.$(NC)"
	@/bin/rm -rf $(OBJ_PATH)

fclean :	clean
	@echo "$(YELLOW)$(NAME) Deleted.$(NC)"
	@/bin/rm -f $(NAME)

re :	fclean all

rc :
	make -C $(LIBFT_PATH) fclean && make fclean && make