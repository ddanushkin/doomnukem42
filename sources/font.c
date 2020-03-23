/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   font.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/23 17:27:04 by lglover           #+#    #+#             */
/*   Updated: 2020/03/23 17:27:04 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	font_reset(t_app *app)
{
	app->font_size = 0;
	app->font_color = 0xffffff;
}

void	font_set(t_app *app, int size, uint32_t color)
{
	app->font_size = size;
	app->font_color = color;
}
