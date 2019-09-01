/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/04 18:40:12 by lglover           #+#    #+#             */
/*   Updated: 2018/12/21 13:29:22 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_list_clear_alt(t_list *list)
{
	t_list		*next;

	if (!list)
		return ;
	next = list->next;
	free(list->content);
	list->content = NULL;
	free(list);
	ft_list_clear_alt(next);
}

static void	ft_list_clear(t_list **begin_list)
{
	ft_list_clear_alt(*begin_list);
	*begin_list = NULL;
}

static void	ft_lstappend(t_list *head, t_list *elem)
{
	t_list	*last_element;

	if (head == NULL || elem == NULL)
		return ;
	last_element = head;
	while (last_element->next != NULL)
		last_element = last_element->next;
	last_element->next = elem;
}

t_list		*ft_lstmap(t_list *lst, t_list *(*f)(t_list *elem))
{
	t_list *new_head;
	t_list *new_elem;
	t_list *tmp;

	if (lst == NULL || f == NULL)
		return (NULL);
	new_elem = f(lst);
	if (new_elem == NULL)
		return (NULL);
	new_head = new_elem;
	tmp = lst->next;
	while (tmp != NULL)
	{
		new_elem = f(tmp);
		if (new_elem == NULL)
		{
			ft_list_clear(&new_head);
			return (NULL);
		}
		ft_lstappend(new_head, new_elem);
		tmp = tmp->next;
	}
	return (new_head);
}
