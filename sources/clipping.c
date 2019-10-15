#include "doom_nukem.h"

void	set_v0_v1(t_clip_plane *p)
{
	p[0].v0.x = 1.0f;
	p[0].v0.y = 0.0f;
	p[0].v0.z = 0.0f;
	p[0].v1.x = 0.0f;
	p[0].v1.y = 1.0f;
	p[0].v1.z = 0.0f;

	p[1].v0.x = 0.0f;
	p[1].v0.y = SCREEN_H - 1;
	p[1].v0.z = 0.0f;
	p[1].v1.x = 0.0f;
	p[1].v1.y = -1.0f;
	p[1].v1.z = 0.0f;

	p[2].v0.x = 0.0f;
	p[2].v0.y = 1.0f;
	p[2].v0.z = 0.0f;
	p[2].v1.x = 1.0f;
	p[2].v1.y = 0.0f;
	p[2].v1.z = 0.0f;

	p[3].v0.x = SCREEN_W - 1;
	p[3].v0.y = 0.0f;
	p[3].v0.z = 0.0f;
	p[3].v1.x = -1.0f;
	p[3].v1.y = 0.0f;
	p[3].v1.z = 0.0f;
}

void	pop_front(t_tr_list **list)
{
	t_tr_list *prev = NULL;
	t_triangle val;
	if (list == NULL) {
		exit(-1);
	}
	prev = (*list);
	val = prev->tr;
	(*list) = (*list)->next;
//	free(prev);
}

double		dist(t_vector vert, t_vector vert0, t_vector vert1)
{
	t_vector temp;
	double r;

	temp = vector_normalise(vert);

	double p = vector_dot_product(vert1, vert0);
	if (vert1.y < 0 && vert.y > SCREEN_H)
		p *= -1;
	if (vert1.x < 0 && vert.x > SCREEN_H)
		p *= -1;
	r = (vector_dot_product(vert1, temp) - p);
	return (r);
}

t_vector	vector_inter_plan(t_vector vert0, t_vector vert1, t_vector line_start, t_vector line_end)
{
	vert1 = vector_normalise(vert1);
	double d = -vector_dot_product(vert1, vert0);
	double ad = vector_dot_product(line_start, vert1);
	double bd = vector_dot_product(line_end, vert1);
	double t = (-d - ad) / (bd - ad);
	t_vector line_s_to_end = vector_sub(line_end, line_start);
	t_vector line_to_inter = vector_mul_by(line_s_to_end, t);
	return (vector_sum(line_start, line_to_inter));
}

int		clip_clip_triangle(t_vector vert0, t_vector vert1, t_triangle temp, t_triangle *tr_out1, t_triangle *tr_out2)
{
	double		d0, d1, d2;
	t_vector in_point[3];
	t_vector out_point[3];
	int		in_point_count;
	int		out_point_count;

	in_point_count = 0;
	out_point_count = 0;
	d0 = dist(temp.v[0], vert0, vert1);
	d1 = dist(temp.v[1], vert0, vert1);
	d2 = dist(temp.v[2], vert0, vert1);
	if (d0 >= 0)
		in_point[in_point_count++] = temp.v[0];
	else
		out_point[out_point_count++] = temp.v[0];
	if (d1 >= 0)
		in_point[in_point_count++] = temp.v[1];
	else
		out_point[out_point_count++] = temp.v[1];
	if (d2 >= 0)
		in_point[in_point_count++] = temp.v[2];
	else
		out_point[out_point_count++] = temp.v[2];
	if (in_point_count == 0)
		return (0);
	if (in_point_count == 3)
	{
		*tr_out1 = temp;
		return (1);
	}
	if (in_point_count == 1 && out_point_count == 2)
	{
		tr_out1->v[0] = in_point[0];
		tr_out1->v[1] = vector_inter_plan(vert0, vert1, in_point[0], out_point[0]);
		tr_out1->v[2] = vector_inter_plan(vert0, vert1, in_point[0], out_point[1]);
		return (1);
	}
	if (in_point_count == 2 && out_point_count == 1)
	{
		tr_out1->v[0] = in_point[0];
		tr_out1->v[1] = in_point[1];
		tr_out1->v[2] = vector_inter_plan(vert0, vert1, in_point[0], out_point[0]);

		tr_out2->v[0] = in_point[1];
		tr_out2->v[1] = tr_out1->v[2];
		tr_out2->v[2] = vector_inter_plan(vert0, vert1, in_point[1], out_point[0]);
		return (2);
	}
	return 0;
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
	t_tr_list	*tmp;

	tmp = write_lst_elem(to_add);
	if ((last = get_last(*tr_lst)) == NULL)
	{
		*tr_lst = tmp;
		return;
	}
	last->next = tmp;
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

void	clip_triangle1(t_app *app, t_tr_list *tr_lst)
{
	int				new_tr;
	int				p;
	int				tr_to_add;
	int 			count;
	t_triangle		test;
	t_triangle		clip[2];
	t_clip_plane 	cp[4];

	new_tr = 1;
	p = 0;
	set_v0_v1(&cp[0]);

	while (p < 4)
	{
		while (new_tr > 0)
		{
			new_tr--;
			if (tr_lst == NULL)
			{
				printf("NULL list in clip1");
				exit(2);
			}
			test = tr_lst->tr;
			pop_front(&tr_lst);
			tr_to_add = clip_clip_triangle(cp[p].v0, cp[p].v1, test, &clip[0], &clip[1]);
			count = 0;
			while (count < tr_to_add)
			{
				push_back(&tr_lst, clip[count]);
				count++;
			}
		}
		new_tr = size_lst(tr_lst);
		p++;
	}
}