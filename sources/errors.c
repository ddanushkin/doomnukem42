/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lglover <lglover@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/11 13:03:29 by lglover           #+#    #+#             */
/*   Updated: 2020/03/11 14:29:58 by lglover          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	exit_with_status(int status, char *fnf_path)
{
	if (status == STATUS_FILE_NOT_FOUND)
	{
		ft_putstr(MSG_FILE_NOT_FOUND);
		ft_putendl(fnf_path);
	}
	if (status == STATUS_BAD_RESOURCES)
		ft_putendl(MSG_BAD_RESOURCES);
	exit(0);
}

void	quit_properly(void)
{
	exit(0);
}
