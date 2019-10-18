#include <doom_nukem.h>

t_list	*new_vertex_elem()
{
	t_v3d	v;

	v = new_vector(0.0f, 0.0f, 0.0f);
	printf("!\n");
	return (ft_lstnew(&v, sizeof(t_v3d)));
}

t_triangle	new_triangle(t_v3d v0, t_v3d v1, t_v3d v2)
{
	t_triangle	t;
	t_color		c;

	t.v[0] = v0;
	t.v[1] = v1;
	t.v[2] = v2;
	set_color(&c, 128, 128, 128);
	t.color = c;
	t.visible = 0;
	return (t);
}

t_tr_list	*new_triangle_elem()
{
	t_v3d	v;
	t_tr_list	*elem;

	v = new_vector(0.0f, 0.0f, 0.0f);
	elem = (t_tr_list *)malloc(sizeof(t_tr_list));
	elem->tr = new_triangle(v, v, v);
	elem->last = 0;
	elem->next = NULL;
	return (elem);
}

t_tr_list	*new_triangle_list(int len)
{
	t_tr_list	*head;
	t_tr_list	*cursor;

	head = new_triangle_elem();
	cursor = head;
	while (--len > 0)
	{
		cursor->next = new_triangle_elem();
		cursor = cursor->next;
	}
	cursor->next = NULL;
	return (head);
}