/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/11 13:03:38 by lglover           #+#    #+#             */
/*   Updated: 2020/03/11 15:34:10 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void		vertex_perspective_divide(t_v3d *v)
{
	v->x /= v->w;
	v->y /= v->w;
	v->z /= v->w;
}

t_mat4x4	get_transform_matrix(t_mat4x4 view_projection)
{
	return (matrix_multiply(
			view_projection,
			matrix_multiply(
					matrix_translation(0, 0, 0),
					matrix_rotation(0.0, 0.0, 0.0))));
}
