/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_powd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndremora <ndremora@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 16:17:34 by ndremora          #+#    #+#             */
/*   Updated: 2019/04/16 12:59:32 by ndremora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

double		ft_powd(double num, int pow)
{
	double	out;

	out = num;
	if (pow == 0)
		return (1);
	while (--pow)
		out *= num;
	return (out);
}
