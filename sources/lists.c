#include <doom_nukem.h>

t_list	*new_vertex_elem()
{
	t_vector	v;

	v = new_vector(0.0f, 0.0f, 0.0f);
	printf("!\n");
	return (ft_lstnew(&v, sizeof(t_vector)));
}

t_list	*new_triangle_elem(t_vector v0, t_vector v1, t_vector v2)
{
	t_triangle	t;
	t_color		c;

	t.v[0] = v0;
	t.v[1] = v1;
	t.v[2] = v2;
	set_color(&c, 128, 128, 128);
	t.color = c;
	t.visible = 0;
	return (ft_lstnew(&t, sizeof(t_triangle)));
}

t_list	*new_triangle_list(int len)
{
	t_list	*head;
	t_vector v;

	v = new_vector(0.0f, 0.0f, 0.0f);
	head = new_triangle_elem(v, v, v);
	while (--len > 0)
		ft_lstadd(&head, new_triangle_elem(v, v, v));
	return (head);
}