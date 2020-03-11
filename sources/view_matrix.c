/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   view_matrix.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/11 14:51:44 by lglover           #+#    #+#             */
/*   Updated: 2020/03/11 14:51:44 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void		view_matrix_data_calc(t_vm_data *d, double pitch, double yaw)
{
	d->cosp = cos(pitch);
	d->sinp = sin(pitch);
	d->cosy = cos(yaw);
	d->siny = sin(yaw);
	d->xa = new_vector(d->cosy, 0.0, -d->siny);
	d->ya = new_vector(d->siny * d->sinp, d->cosp, d->cosy * d->sinp);
	d->za = new_vector(d->siny * d->cosp, -d->sinp, d->cosp * d->cosy);
}

t_mat4x4	view_matrix(t_v3d eye, double pitch, double yaw)
{
	t_vm_data	vmd;
	t_mat4x4	view;

	view_matrix_data_calc(&vmd, pitch, yaw);
	view.m[0] = vmd.xa.x;
	view.m[1] = vmd.xa.y;
	view.m[2] = vmd.xa.z;
	view.m[3] = -v3d_dot(vmd.xa, eye);
	view.m[4] = vmd.ya.x;
	view.m[5] = vmd.ya.y;
	view.m[6] = vmd.ya.z;
	view.m[7] = -v3d_dot(vmd.ya, eye);
	view.m[8] = vmd.za.x;
	view.m[9] = vmd.za.y;
	view.m[10] = vmd.za.z;
	view.m[11] = -v3d_dot(vmd.za, eye);
	view.m[12] = 0.0;
	view.m[13] = 0.0;
	view.m[14] = 0.0;
	view.m[15] = 1.0;
	return (view);
}
