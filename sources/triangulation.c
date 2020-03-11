/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   triangulation.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/11 13:03:47 by lglover           #+#    #+#             */
/*   Updated: 2020/03/11 17:04:57 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

t_polygon	*biggest_ear(t_polygon *p)
{
	t_polygon	*head;
	t_polygon	*ear;

	ear = NULL;
	head = p;
	while (p->next != head)
	{
		if (p->is_ear && ear == NULL)
			ear = p;
		else if (p->is_ear && ear != NULL)
		{
			if (p->angle > ear->angle)
				ear = p;
		}
		p = p->next;
	}
	return (ear);
}

void		update_vertex(t_polygon *v1, t_polygon *p)
{
	t_polygon *v0;
	t_polygon *v2;
	t_polygon *head;

	v0 = v1->prev;
	v2 = v1->next;
	v1->angle = get_angle(v0->v, v1->v, v2->v);
	if (is_convex(v0->v, v1->v, v2->v))
	{
		v1->is_ear = 1;
		head = p;
		while (p->next != head)
		{
			if (p != v0 && p != v1 && p != v2 &&
				is_inside(p->v, v0->v, v1->v, v2->v))
			{
				v1->is_ear = 0;
				break ;
			}
			p = p->next;
		}
	}
	else
		v1->is_ear = 0;
}

void		update_polygon(t_polygon *p)
{
	t_polygon *head;

	head = p;
	while (p->next != head)
	{
		update_vertex(p, head);
		p = p->next;
	}
}

void		triangulate(t_triangle *trs, int *trs_size, t_polygon *polygon)
{
	t_polygon	*prev;
	t_polygon	*next;
	t_polygon	*ear;
	t_triangle	*tr;

	update_polygon(polygon);
	while (polygon_size(polygon) != 3)
	{
		ear = biggest_ear(polygon);
		prev = ear->prev;
		next = ear->next;
		tr = &trs[*trs_size];
		tr->v[0] = prev->v;
		tr->v[1] = ear->v;
		tr->v[2] = next->v;
		trs_size[0]++;
		polygon_delete(&polygon, ear);
		update_vertex(prev, polygon);
		update_vertex(next, polygon);
	}
	tr = &trs[*trs_size];
	tr->v[0] = polygon->v;
	tr->v[1] = polygon->next->v;
	tr->v[2] = polygon->next->next->v;
	trs_size[0]++;
}
