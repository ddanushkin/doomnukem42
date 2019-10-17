#include "doom_nukem.h"

void	set_clip_planes(t_plane *p)
{
	p[0].p = new_vector(0, 0, 0);
	p[0].n = new_vector(0, 1, 0);

	p[1].p = new_vector(0, SCREEN_H - 100.0, 0);
	p[1].n = new_vector(0, -1, 0);

	p[2].p = new_vector(0, 0, 0);
	p[2].n = new_vector(1, 0, 0);

	p[3].p = new_vector(SCREEN_W - 1.0, 0, 0);
	p[3].n = new_vector(-1, 0, 0);
}

void	pop_front(t_tr_list **list)
{
	t_tr_list *tmp;

	tmp = NULL;
	if (list == NULL || *list == NULL)
		exit(-1);
	tmp = (*list)->next;
	free(*list);
	(*list) = tmp;
}

double		dist(t_vector vert, t_plane plane)
{
	t_vector	temp;
	double		r;

	temp = vector_normalise(vert);
	double p = vector_dot_product(plane.n, plane.p);
	if (plane.n.y < 0 && vert.y > SCREEN_H - 100)
		p *= -1;
	if (plane.n.x < 0 && vert.x > SCREEN_W - 1)
		p *= -1;
	r = (vector_dot_product(plane.n, temp) - p);
	return (r);
}

t_vector	vector_inter_plan(t_plane plane, t_vector line_start, t_vector line_end)
{
	plane.n = vector_normalise(plane.n);
	double d = -vector_dot_product(plane.n, plane.p);
	double ad = vector_dot_product(line_start, plane.n);
	double bd = vector_dot_product(line_end, plane.n);
	double t = (-d - ad) / (bd - ad);
	t_vector line_s_to_end = vector_sub(line_end, line_start);
	t_vector line_to_inter = vector_mul_by(line_s_to_end, t);
	return (vector_sum(line_start, line_to_inter));
}

t_tr_list	*write_lst_elem(t_triangle data)
{
	t_tr_list	*tmp;

	tmp = (t_tr_list *)malloc(sizeof(t_tr_list));
	tmp->next = NULL;
	tmp->tr = data;
	return (tmp);
}

t_tr_list	*get_last(t_tr_list *head)
{
	if (head == NULL)
	{
		return NULL;
	}
	while (head->next) {
		head = head->next;
	}
	return head;
}

void	push_back(t_tr_list **tr_lst, t_triangle to_add)
{
	t_tr_list	*last;

	last = get_last(*tr_lst);
	if (last == NULL)
	{
		(*tr_lst) = write_lst_elem(to_add);
		return;
	}
	last->next = write_lst_elem(to_add);
}

int		clip_clip_triangle(t_plane plane, t_tr_list **list)
{
	double		d0, d1, d2;
	t_vector in_point[3];
	t_vector out_point[3];
	int		in_point_count;
	int		out_point_count;
	t_triangle tr;

	tr = (*list)->tr;
	pop_front(list);
	in_point_count = 0;
	out_point_count = 0;
	d0 = dist(tr.v[0], plane);
	d1 = dist(tr.v[1], plane);
	d2 = dist(tr.v[2], plane);
	if (d0 >= 0)
		in_point[in_point_count++] = tr.v[0];
	else
		out_point[out_point_count++] = tr.v[0];
	if (d1 >= 0)
		in_point[in_point_count++] = tr.v[1];
	else
		out_point[out_point_count++] = tr.v[1];
	if (d2 >= 0)
		in_point[in_point_count++] = tr.v[2];
	else
		out_point[out_point_count++] = tr.v[2];
	if (in_point_count == 0)
		return (0);
	if (in_point_count == 3)
	{
		push_back(list, tr);
		return (1);
	}
	if (in_point_count == 1 && out_point_count == 2)
	{
		t_triangle tr_new_1;

		tr_new_1.v[0] = in_point[0];
		tr_new_1.v[1] = vector_inter_plan(plane, in_point[0], out_point[0]);
		tr_new_1.v[2] = vector_inter_plan(plane, in_point[0], out_point[1]);
		tr_new_1.color = new_color(128, 128, 128);
		push_back(list, tr_new_1);
		return (1);
	}
	if (in_point_count == 2 && out_point_count == 1)
	{
		t_triangle tr_new_1;
		t_triangle tr_new_2;

		tr_new_1.v[0] = in_point[0];
		tr_new_1.v[1] = in_point[1];
		tr_new_1.v[2] = vector_inter_plan(plane, in_point[0], out_point[0]);
		tr_new_2.v[0] = in_point[1];
		tr_new_2.v[1] = tr_new_1.v[2];
		tr_new_2.v[2] = vector_inter_plan(plane, in_point[1], out_point[0]);
		tr_new_1.color = new_color(128, 128, 128);
		tr_new_2.color = new_color(128, 128, 128);
		push_back(list, tr_new_1);
		push_back(list, tr_new_2);
		return (2);
	}
	return 0;
}

int		size_lst(t_tr_list *temp)
{
	int		count;

	count = 0;
	while (temp != NULL)
	{
		count++;
		temp = temp->next;
	}
	return (count);
}

void	clip_triangle(t_tr_list **tr_lst)
{
	int			new_tr;
	int			p;
	t_plane 	cp[4];

	new_tr = 1;
	p = 0;
	set_clip_planes(&cp[0]);
	while (p < 4)
	{
		while (new_tr-- > 0)
			clip_clip_triangle(cp[p], tr_lst);
		new_tr = size_lst(*tr_lst);
		p++;
	}
}