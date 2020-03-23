/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clipping.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/23 17:26:23 by lglover           #+#    #+#             */
/*   Updated: 2020/03/23 17:26:23 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

t_v3d	lerp(t_clip_data *prev, t_clip_data *curr)
{
	t_v3d	result;
	double	amount;
	double	tmp;

	tmp = prev->v.w - prev->value;
	amount = (tmp) / ((tmp) - (curr->v.w - curr->value));
	result.x = (curr->v.x - prev->v.x) * amount + prev->v.x;
	result.y = (curr->v.y - prev->v.y) * amount + prev->v.y;
	result.z = (curr->v.z - prev->v.z) * amount + prev->v.z;
	result.w = (curr->v.w - prev->v.w) * amount + prev->v.w;
	result.tex_x = (curr->v.tex_x - prev->v.tex_x) * amount + prev->v.tex_x;
	result.tex_y = (curr->v.tex_y - prev->v.tex_y) * amount + prev->v.tex_y;
	return (result);
}

void	clip_axis(t_vr_list **s_list, double f, t_vr_list **e_list, uint8_t i)
{
	t_vr_list	*cursor;
	t_clip_data	prev;
	t_clip_data	curr;

	cursor = (*s_list);
	prev.v = vr_list_last(*s_list)->v;
	prev.value = ((double *)(&prev.v))[i] * f;
	prev.is_inside = prev.value <= prev.v.w;
	while (cursor)
	{
		curr.v = cursor->v;
		curr.value = ((double *)(&curr.v))[i] * f;
		curr.is_inside = curr.value <= curr.v.w;
		if (curr.is_inside ^ prev.is_inside)
			vr_list_add(e_list, lerp(&prev, &curr));
		if (curr.is_inside)
			vr_list_add(e_list, curr.v);
		SWAP(prev, curr, t_clip_data);
		cursor = cursor->next;
	}
}

int		clip_by_axis(t_vr_list **s_list, t_vr_list **e_list, uint8_t i)
{
	clip_axis(s_list, 1.0, e_list, i);
	vr_list_free(s_list);
	if ((*e_list) == NULL)
		return (0);
	clip_axis(e_list, -1.0, s_list, i);
	vr_list_free(e_list);
	return ((*s_list) != NULL);
}

void	clip_triangle(t_app *app, t_vr_list *list)
{
	t_v3d		root;
	t_vr_list	*node;

	root = list->v;
	node = list;
	list = list->next;
	free(node);
	while (list->next)
	{
		node = list;
		fill_triangle(app, root, node->v, node->next->v);
		list = list->next;
		free(node);
	}
	free(list);
}

void	clip_fill_triangle(t_app *app, t_v3d v1, t_v3d v2, t_v3d v3)
{
	t_vr_list	*start_list;
	t_vr_list	*end_list;

	start_list = NULL;
	end_list = NULL;
	vr_list_add(&start_list, v1);
	vr_list_add(&start_list, v2);
	vr_list_add(&start_list, v3);
	if (clip_by_axis(&start_list, &end_list, 0) &&
		clip_by_axis(&start_list, &end_list, 1) &&
		clip_by_axis(&start_list, &end_list, 2))
	{
		clip_triangle(app, start_list);
	}
}
