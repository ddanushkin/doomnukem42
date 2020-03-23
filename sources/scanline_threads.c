/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanline_threads.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/23 17:28:34 by lglover           #+#    #+#             */
/*   Updated: 2020/03/23 17:28:34 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

uint32_t	shade(uint8_t shade, uint32_t c)
{
	int		r;
	int		g;
	int		b;

	r = (int)((c >> 16) & 0xFF) - shade;
	if (r < 0)
		r = 0;
	g = (int)((c >> 8) & 0xFF) - shade;
	if (g < 0)
		g = 0;
	b = (int)(c & 0xFF) - shade;
	if (b < 0)
		b = 0;
	return ((r << 16u) | (g << 8u) | b);
}

void		scanline_draw(register t_sl_data *s, register uint32_t *t,
		register double *depth, register uint32_t *screen)
{
	int			offset;
	uint32_t	c;
	double		tmp_z;

	offset = s->offset;
	while (s->start++ < s->end)
	{
		if (depth[offset] - s->d > 0.00001)
		{
			tmp_z = 1.0 / s->z;
			c = t[((uint8_t)(s->y * tmp_z) << 8u) + (uint8_t)(s->x * tmp_z)];
			if (c != TRANSPARENCY_COLOR)
			{
				depth[offset] = s->d;
				screen[offset] = s->shade != 0 ? shade(s->shade, c) : c;
			}
		}
		s->z += s->zs;
		s->x += s->xs;
		s->y += s->ys;
		s->d += s->ds;
		offset++;
	}
}

void		thr_data_set(t_thr_data *thr_data, t_render *r, int start, int end)
{
	thr_data->start = start;
	thr_data->end = end;
	thr_data->r = r;
}

void		*scanline_thr(register void *ptr)
{
	t_thr_data			*td;
	register t_render	*r;
	int					i;
	int					len;

	td = (t_thr_data *)ptr;
	r = td->r;
	i = td->start;
	len = td->end;
	while (i < len)
		scanline_draw(&r->sl[i++], r->t, r->depth, r->screen);
	return (NULL);
}

void		scanline_threads(register t_render *r, int size)
{
	int			start;
	int			step;
	int			i;
	pthread_t	thr[THREADS_N];
	t_thr_data	thr_data[THREADS_N];

	start = 0;
	step = size / (THREADS_N - 1);
	i = 0;
	while (i < (THREADS_N - 1) && step > 0)
	{
		thr_data_set(&thr_data[i], r, start, start + step);
		pthread_create(&thr[i], NULL, scanline_thr, &thr_data[i]);
		start += step;
		i++;
	}
	if (start < size)
	{
		thr_data_set(&thr_data[i], r, start, size);
		pthread_create(&thr[i], NULL, scanline_thr, &thr_data[i]);
	}
	i = 0;
	while (i < THREADS_N)
		pthread_join(thr[i++], NULL);
}
