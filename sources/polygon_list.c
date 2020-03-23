/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   polygon_list.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/23 17:28:17 by lglover           #+#    #+#             */
/*   Updated: 2020/03/23 17:28:17 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	polygon_add(t_polygon **poly, t_v3d v)
{
	t_polygon	*new;
	t_polygon	*last;

	if ((*poly) == NULL)
	{
		(*poly) = (t_polygon *)malloc(sizeof(t_polygon));
		(*poly)->v = v;
		(*poly)->is_ear = 0;
		(*poly)->angle = 0.0;
		(*poly)->next = *poly;
		(*poly)->prev = *poly;
	}
	else
	{
		last = (*poly)->prev;
		new = (t_polygon *)malloc(sizeof(t_polygon));
		new->v = v;
		new->is_ear = 0;
		new->angle = 0.0;
		new->next = (*poly);
		new->prev = last;
		last->next = new;
		(*poly)->prev = new;
	}
}

void	polygon_delete(t_polygon **p, t_polygon *del)
{
	t_polygon	*prev;
	t_polygon	*next;
	t_polygon	*head;

	head = *p;
	if (*p == del)
	{
		prev = (*p)->prev;
		next = (*p)->next;
		(*p)->prev->next = next;
		(*p)->next->prev = prev;
		*p = head->next;
		free(del);
		return ;
	}
	while ((*p) != del)
		*p = (*p)->next;
	prev = (*p)->prev;
	next = (*p)->next;
	(*p)->prev->next = next;
	(*p)->next->prev = prev;
	*p = head;
	free(del);
}

int		polygon_size(t_polygon *p)
{
	int			size;
	t_polygon	*head;

	if (p == NULL)
		return (0);
	size = 1;
	head = p;
	while (p->next != head)
	{
		size++;
		p = p->next;
	}
	return (size);
}
