#include "doom_nukem.h"

t_v3d	lerp(t_clip_data *prev, t_clip_data *curr)
{
	t_v3d	result;
	double	amount;
	double	tmp;

	tmp = prev->v.w - prev->value;
	amount = (tmp) / ((tmp) - (curr->v.w - curr->value));
	result.x = (curr->v.x - prev->v.x) * amount + prev->v.x;
	result.y = (curr->v.y - prev->v.y) * amount + prev->v.y;
	result.z = (curr->v.z - prev->v.z) * amount + prev->v.z;
	result.w = (curr->v.w - prev->v.w) * amount + prev->v.w;
	result.tex_x = (curr->v.tex_x - prev->v.tex_x) * amount + prev->v.tex_x;
	result.tex_y = (curr->v.tex_y - prev->v.tex_y) * amount + prev->v.tex_y;
	return (result);
}

void 	clip_axis(t_vr_list **start_list, double factor, t_vr_list **end_list, Uint8 index)
{
	t_vr_list	*cursor;
	t_clip_data	prev;
	t_clip_data	curr;

	cursor = (*start_list);
	prev.v = vr_list_last(*start_list)->v;
	prev.value = ((double *)(&prev.v))[index] * factor;
	prev.is_inside = prev.value <= prev.v.w;
	while (cursor)
	{
		curr.v = cursor->v;
		curr.value = ((double *)(&curr.v))[index] * factor;
		curr.is_inside = curr.value <= curr.v.w;
		if (curr.is_inside ^ prev.is_inside)
			vr_list_add(end_list, lerp(&prev, &curr));
		if (curr.is_inside)
			vr_list_add(end_list, curr.v);
		SWAP(prev, curr, t_clip_data);
		cursor = cursor->next;
	}
}

int 	clip_by_axis(t_vr_list **start_list, t_vr_list **end_list, Uint8 index)
{
	clip_axis(start_list, 1.0, end_list, index);
	vr_list_free(start_list);
	if ((*end_list) == NULL)
		return (0);
	clip_axis(end_list, -1.0, start_list, index);
	vr_list_free(end_list);
	return ((*start_list) != NULL);
}

void	clip_triangle(t_app *app, t_vr_list	*list)
{
	t_v3d		root;
	t_vr_list	*node;

	root = list->v;
	node = list;
	list = list->next;
	free(node);
	while (list->next)
	{
		node = list;
		fill_triangle(app, root, node->v, node->next->v);
		list = list->next;
		free(node);
	}
	free(list);
}

void 	clip_fill_triangle(t_app *app, t_v3d v1, t_v3d v2, t_v3d v3)
{
	t_vr_list	*start_list;
	t_vr_list	*end_list;

	start_list = NULL;
	end_list = NULL;
	vr_list_add(&start_list, v1);
	vr_list_add(&start_list, v2);
	vr_list_add(&start_list, v3);
	if (clip_by_axis(&start_list, &end_list, 0) &&
		clip_by_axis(&start_list, &end_list, 1) &&
		clip_by_axis(&start_list, &end_list, 2))
	{
		clip_triangle(app, start_list);
	}
}