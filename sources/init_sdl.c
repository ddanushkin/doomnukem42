/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_sdl.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/11 15:39:42 by lglover           #+#    #+#             */
/*   Updated: 2020/03/11 15:42:41 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	init_sdl(t_sdl *sdl)
{
	sdl->width = SCREEN_W;
	sdl->height = SCREEN_H;
	SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO);
	TTF_Init();
	Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);
	Mix_AllocateChannels(16);
	Mix_VolumeMusic(MIX_MAX_VOLUME / 8);
	Mix_Volume(-1, MIX_MAX_VOLUME / 8);
	sdl->window = SDL_CreateWindow(
			WIN_TITLE,
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			sdl->width,
			sdl->height,
			SDL_WINDOW_OPENGL);
	sdl->surface = SDL_GetWindowSurface(sdl->window);
}

void	init_font(t_app *app)
{
	SDL_RWops	*raw;

	raw = SDL_RWFromMem(&app->fonts.mem[0], app->fonts.size);
	app->font = TTF_OpenFontRW(raw, 1, 16);
}

void	init_sfx(t_app *app)
{
	SDL_RWops	*raw;
	int			i;

	i = 0;
	app->sfx = (Mix_Chunk **)malloc(sizeof(Mix_Chunk *) * MAX_SFX);
	while (i < MAX_SFX)
	{
		raw = SDL_RWFromMem(&app->audio[i].mem[0], app->audio[i].size);
		app->sfx[i] = Mix_LoadWAV_RW(raw, 1);
		i++;
	}
}

void	init_bg(t_app *app)
{
	SDL_RWops	*raw;
	int			i;

	i = 0;
	app->bg = (Mix_Music **)malloc(sizeof(Mix_Music *) * MAX_BG);
	while (i < MAX_BG)
	{
		raw = SDL_RWFromMem(&app->music[i].mem[0], app->music[i].size);
		app->bg[i] = Mix_LoadMUSType_RW(raw, MUS_OGG, 1);
		i++;
	}
}
