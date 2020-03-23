/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   room_polygon_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/23 17:28:32 by lglover           #+#    #+#             */
/*   Updated: 2020/03/23 17:28:32 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

double	get_orientation(t_v3d *polygon, int size)
{
	int		i;
	double	sum;
	t_v3d	v1;
	t_v3d	v2;

	i = 0;
	sum = 0.0;
	while (i < size)
	{
		v1 = polygon[i];
		v2 = polygon[(i + 1) % size];
		sum += v1.x * v2.z - v1.z * v2.x;
		i++;
	}
	return (sum);
}
