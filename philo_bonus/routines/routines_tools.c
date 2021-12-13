/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines_tools.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchopin <gchopin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/17 17:20:05 by gchopin           #+#    #+#             */
/*   Updated: 2021/12/13 15:37:37 by gchopin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher_bonus.h"

void	stop_routine(t_philosopher *philo, int result)
{
	if (result != 0)
	{
		philo->dead = 1;
		sem_post(philo->wait_loop);
		exit(EXIT_FAILURE);
	}
}
