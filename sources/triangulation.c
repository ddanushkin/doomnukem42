#include "doom_nukem.h"

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
		return;
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

int 	polygon_size(t_polygon *p)
{
	int size;
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

int		is_convex(t_v3d p1, t_v3d p2, t_v3d p3)
{
	return (((p3.z - p1.z) * (p2.x - p1.x) -
			 (p3.x - p1.x) * (p2.z - p1.z)) > 0);
}

int 	is_inside(t_v3d p, t_v3d p1, t_v3d p2, t_v3d p3)
{
	if (is_convex(p1, p, p2))
		return (0);
	if (is_convex(p2, p, p3))
		return (0);
	if (is_convex(p3, p, p1))
		return (0);
	return 1;
}

t_v3d 	normalize(t_v3d v)
{
	t_v3d	res;
	double	len;

	res = new_vector(0, 0, 0);
	len = sqrt(v.x * v.x + v.z * v.z);
	if (len != 0)
		res = vector_div_by(v, len);
	return (res);
}

double	get_angle(t_v3d v0, t_v3d v1, t_v3d v2)
{
	t_v3d	vec1;
	t_v3d	vec2;

	vec1 = normalize(vector_sub(v0, v1));
	vec2 = normalize(vector_sub(v2, v1));
	return (vec1.x * vec2.x + vec1.z * vec2.z);
}

void	update_vertex(t_polygon *v1, t_polygon *p)
{
	t_polygon *v0;
	t_polygon *v2;
	t_polygon *head;

	v0 = v1->prev;
	v2 = v1->next;
	v1->angle = get_angle(v0->v, v1->v, v2->v);
	if (is_convex(v0->v, v1->v, v2->v))
	{
		v1->is_convex = 1;
		v1->is_ear = 1;
		head = p;
		while (p->next != head)
		{
			if (p == v0 || p == v1 || p == v2)
			{
				p = p->next;
				continue;
			}
			if (is_inside(p->v, v0->v, v1->v, v2->v))
			{
				v1->is_ear = 0;
				break;
			}
			p = p->next;
		}
	}
	else
		v1->is_ear = 0;
}

void	update_polygon(t_polygon *p)
{
	t_polygon *head;

	head = p;
	while (p->next != head)
	{
		update_vertex(p, head);
		p = p->next;
	}
}

void	print_polygon(t_polygon *p)
{
	t_polygon *head;

	printf("... -> ");
	head = p;
	while (p->next != head)
	{
		printf("[%f, %f, %f] -> ", p->v.x, p->v.y, p->v.z);
		p = p->next;
	}
	printf("[%f, %f, %f] -> ", p->v.x, p->v.y, p->v.z);
	printf("...\n");
}

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
			if(p->angle > ear->angle) {
				ear = p;
			}
		}
		p = p->next;
	}
	return (ear);
}

void	triangulate(t_sector *cs)
{
	t_polygon	*prev;
	t_polygon	*next;
	t_polygon	*ear;
	t_triangle	*tr;

	update_polygon(cs->polygon);
	while (polygon_size(cs->polygon) != 3)
	{
		ear = biggest_ear(cs->polygon);
		prev = ear->prev;
		next = ear->next;
		tr = &cs->triangles[cs->triangles_count];
		tr->v[0] = prev->v;
		tr->v[1] = ear->v;
		tr->v[2] = next->v;
		cs->triangles_count++;
		polygon_delete(&cs->polygon, ear);
		update_vertex(prev, cs->polygon);
		update_vertex(next, cs->polygon);
	}
	tr = &cs->triangles[cs->triangles_count];
	tr->v[0] = cs->polygon->v;
	tr->v[1] = cs->polygon->next->v;
	tr->v[2] = cs->polygon->next->next->v;
	cs->triangles_count++;
}