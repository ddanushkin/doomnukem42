#include "doom_nukem.h"

void		set_clip_planes(t_plane *p)
{
	p[0].p = new_vector(0, 0, 0);
	p[0].n = new_vector(0, 1, 0);
	p[1].p = new_vector(0, SCREEN_H - 1.0, 0);
	p[1].n = new_vector(0, -1, 0);
	p[2].p = new_vector(0, 0, 0);
	p[2].n = new_vector(1, 0, 0);
	p[3].p = new_vector(SCREEN_W - 1.0, 0, 0);
	p[3].n = new_vector(-1, 0, 0);
}

void		pop_front(t_tr_list **list)
{
	t_tr_list *tmp;

	if (list == NULL || *list == NULL)
		exit(-1);
	tmp = (*list)->next;
	free(*list);
	(*list) = tmp;
}

void		choose_dist(double dist, t_io *io, t_triangle *tr, int index)
{
	if (dist >= 0)
	{
		io->vin[io->vins] = tr->v[index];
		io->tin[io->vins++] = tr->t[index];
	}
	else
	{
		io->vout[io->vouts] = tr->v[index];
		io->tout[io->vouts++] = tr->t[index];
	}
}

void		set_dist(t_v3d *vert, t_plane plane, t_io *io, t_triangle *tr)
{
	t_v3d	temp;
	t_v3d	v;
	double	p;
	int		i;
	double	dist;

	i = 0;
	while (i < 3)
	{
		v = vert[i];
		p = vector_dot_product(plane.n, plane.p);
		temp = vector_normalise(v);
		if (plane.n.y < 0 && v.y > SCREEN_H - 1)
			p *= -1;
		if (plane.n.x < 0 && v.x > SCREEN_W - 1)
			p *= -1;
		dist = (vector_dot_product(plane.n, temp) - p);
		choose_dist(dist, io, tr, i);
		i++;
	}
}

t_v3d		vector_inter_plan(t_plane plane, t_v3d line_start,
								t_v3d line_end, double *t)
{
	double	d;
	double	ad;
	double	bd;

	plane.n = vector_normalise(plane.n);
	d = -vector_dot_product(plane.n, plane.p);
	ad = vector_dot_product(line_start, plane.n);
	bd = vector_dot_product(line_end, plane.n);
	*t = (-d - ad) / (bd - ad);
	return (vector_sum(line_start,
			vector_mul_by(vector_sub(line_end, line_start), *t)));
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
		return (NULL);
	while (head->next)
		head = head->next;
	return (head);
}

void		push_back(t_tr_list **tr_lst, t_triangle to_add)
{
	t_tr_list	*last;

	last = get_last(*tr_lst);
	if (last == NULL)
	{
		(*tr_lst) = write_lst_elem(to_add);
		return ;
	}
	last->next = write_lst_elem(to_add);
}

void		clip_add_tr_1(t_tr_list **list, t_io *io, t_plane *plane)
{
	t_triangle	tr_new_1;
	double		t;

	tr_new_1.v[0] = io->vin[0];
	tr_new_1.v[1] = vector_inter_plan(*plane, io->vin[0], io->vout[0], &t);
	tr_new_1.v[2] = vector_inter_plan(*plane, io->vin[0], io->vout[1], &t);
	tr_new_1.t[0] = io->tin[0];
	tr_new_1.t[1].u = t * (io->tout[0].u - io->tin[0].u) + io->tin[0].u;
	tr_new_1.t[1].v = t * (io->tout[0].v - io->tin[0].v) + io->tin[0].v;
	tr_new_1.t[1].w = t * (io->tout[0].w - io->tin[0].w) + io->tin[0].w;
	tr_new_1.t[2].u = t * (io->tout[1].u - io->tin[0].u) + io->tin[0].u;
	tr_new_1.t[2].v = t * (io->tout[1].v - io->tin[0].v) + io->tin[0].v;
	tr_new_1.t[2].w = t * (io->tout[1].w - io->tin[0].w) + io->tin[0].w;
	tr_new_1.color = color_new(128, 128, 128);
	push_back(list, tr_new_1);
}

void		clip_add_tr_2(t_tr_list **list, t_io *io, t_plane *plane)
{
	t_triangle	tr_new_1;
	t_triangle	tr_new_2;
	double		t;

	tr_new_1.v[0] = io->vin[0];
	tr_new_1.v[1] = io->vin[1];
	tr_new_1.v[2] = vector_inter_plan(*plane, io->vin[0], io->vout[0], &t);
	tr_new_1.t[0] = io->tin[0];
	tr_new_1.t[1] = io->tin[1];
	tr_new_1.t[2].u = t * (io->tout[0].u - io->tin[0].u) + io->tin[0].u;
	tr_new_1.t[2].v = t * (io->tout[0].v - io->tin[0].v) + io->tin[0].v;
	tr_new_1.t[2].w = t * (io->tout[0].w - io->tin[0].w) + io->tin[0].w;
	tr_new_1.color = color_new(128, 128, 128);
	push_back(list, tr_new_1);
	tr_new_2.v[0] = io->vin[1];
	tr_new_2.v[1] = tr_new_1.v[2];
	tr_new_2.v[2] = vector_inter_plan(*plane, io->vin[1], io->vout[0], &t);
	tr_new_2.t[0] = io->tin[1];
	tr_new_2.t[1] = tr_new_1.t[2];
	tr_new_2.t[2].u = t * (io->tout[0].u - io->tin[1].u) + io->tin[1].u;
	tr_new_2.t[2].v = t * (io->tout[0].v - io->tin[1].v) + io->tin[1].v;
	tr_new_2.t[2].w = t * (io->tout[0].w - io->tin[1].w) + io->tin[1].w;
	tr_new_2.color = color_new(128, 128, 128);
	push_back(list, tr_new_2);
}

void		clip_choose_push(t_tr_list **list, t_io *io, t_plane *plane,
			t_triangle *tr)
{
	if (io->vins == 3)
		push_back(list, *tr);
	if (io->vins == 1 && io->vouts == 2)
		clip_add_tr_1(list, io, plane);
	if (io->vins == 2 && io->vouts == 1)
		clip_add_tr_2(list, io, plane);
}

int			size_lst(t_tr_list *temp)
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

void		clip_triangles(t_tr_list **list)
{
	int			new_tr;
	int			p;
	t_plane		cp[4];
	t_io		io;
	t_triangle	tr;

	new_tr = 1;
	p = 0;
	set_clip_planes(&cp[0]);
	while (p < 4)
	{
		while (new_tr-- > 0)
		{
			tr = (*list)->tr;
			pop_front(list);
			ft_memset(&io, 0, sizeof(t_io));
			set_dist(&tr.v[0], cp[p], &io, &tr);
			clip_choose_push(list, &io, &cp[p], &tr);
		}
		new_tr = size_lst(*list);
		p++;
	}
}
