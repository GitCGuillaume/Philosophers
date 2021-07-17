/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines_tools.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchopin <gchopin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/17 17:20:05 by gchopin           #+#    #+#             */
/*   Updated: 2021/07/17 18:00:28 by gchopin          ###   ########.fr       */
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

void	stop_routine_timer(t_philosopher *philo)
{
	if (philo->state.current_time == -1)
	{
		philo->dead = 1;
		sem_post(philo->wait_loop);
		exit(EXIT_FAILURE);
	}
}

void	stop_routine_timer_sim(t_philosopher *philo)
{
	if (philo->state.time_simulation == -1)
	{
		philo->dead = 1;
		sem_post(philo->wait_loop);
		exit(EXIT_FAILURE);
	}
}
