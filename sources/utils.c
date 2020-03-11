/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/11 14:25:47 by lglover           #+#    #+#             */
/*   Updated: 2020/03/11 15:34:10 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

t_v3d	get_triangle_normal(t_v3d v0, t_v3d v1, t_v3d v2)
{
	return (v3d_normalise(v3d_cross(v3d_sub(v0, v1), v3d_sub(v0, v2))));
}

double	signed_tetra_volume(t_v3d ba, t_v3d ca, t_v3d da)
{
	return (SIGNF(v3d_dot(v3d_cross(ba, ca), da) * 0.166666));
}
