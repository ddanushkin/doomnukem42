/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/11 13:03:29 by lglover           #+#    #+#             */
/*   Updated: 2020/03/11 14:29:58 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	exit_with_status(int status, char *fnf_path)
{
	if (status == STATUS_FILE_NOT_FOUND)
	{
		ft_putstr(MSG_FILE_NOT_FOUND);
		ft_putendl(fnf_path);
	}
	else if (status == STATUS_MAP_NOT_FOUND)
	{
		ft_putstr(MSG_MAP_NOT_FOUND);
		ft_putendl(fnf_path);
	}
	else if (status == STATUS_BAD_RESOURCES)
		ft_putendl(MSG_BAD_RESOURCES);
	else if (status == STATUS_NO_MAP_PATH)
		ft_putendl(MSG_NO_MAP_PATH);
	else if (status == STATUS_NO_GAME_DATA)
		ft_putendl(MSG_NO_GAME_DATA);
	exit(0);
}

void	app_exit(t_app *app)
{
	TTF_CloseFont(app->font);
	TTF_Quit();
	Mix_Quit();
    SDL_DestroyWindow(app->sdl->window);
    SDL_Quit();
	exit(0);
}
