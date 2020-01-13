#include "doom_nukem.h"

void vertex_perspective_divide(t_v3d *v)
{
	v->x /= v->w;
	v->y /= v->w;
	v->z /= v->w;
}

t_mat4x4 	get_transform_matrix(t_mat4x4 view_projection)
{
	return (matrix_multiply(
			view_projection,
			matrix_multiply(
					matrix_translation(0, 0, 0),
					matrix_rotation(0.0, 0.0, 0.0))));
}

