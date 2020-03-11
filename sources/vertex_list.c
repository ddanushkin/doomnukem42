/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vertex_list.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/11 13:03:53 by lglover           #+#    #+#             */
/*   Updated: 2020/03/11 17:12:08 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void		vr_list_add(t_vr_list **list, t_v3d v)
{
	t_vr_list	*head;

	if ((*list) == NULL)
	{
		(*list) = (t_vr_list *)malloc(sizeof(t_vr_list));
		(*list)->v = v;
		(*list)->next = NULL;
	}
	else
	{
		head = *list;
		while ((*list)->next)
			(*list) = (*list)->next;
		(*list)->next = (t_vr_list *)malloc(sizeof(t_vr_list));
		(*list) = (*list)->next;
		(*list)->v = v;
		(*list)->next = NULL;
		*list = head;
	}
}

void		vr_list_free(t_vr_list **list)
{
	t_vr_list *tmp;

	while ((*list) != NULL)
	{
		tmp = (*list);
		(*list) = (*list)->next;
		free(tmp);
	}
	(*list) = NULL;
}

t_vr_list	*vr_list_last(t_vr_list *head)
{
	if (head == NULL)
		return (NULL);
	while (head->next)
		head = head->next;
	return (head);
}
