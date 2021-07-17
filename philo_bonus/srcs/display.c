/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchopin <gchopin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/17 18:49:14 by gchopin           #+#    #+#             */
/*   Updated: 2021/07/17 18:49:15 by gchopin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher_bonus.h"

static void	stop_routine_timer_two(t_philosopher *philo, int result)
{
	if (result == -1)
	{
		philo->dead = 1;
		sem_post(philo->wait_loop);
		exit(EXIT_FAILURE);
	}
}

void	display(t_philosopher *philo, char *str, int is_dead)
{
	long int	current_time;
	int			result;

	current_time = math_time();
	stop_routine_timer_two(philo, current_time);
	if (is_dead == 1)
	{
		printf("%ld %d %s\n",
			current_time - philo->state.start_time, philo->number, str);
		result = sem_post(philo->wait_loop);
		stop_routine(philo, result);
	}
	if (is_dead == 0)
	{
		result = sem_wait(philo->mutex_dead);
		stop_routine(philo, result);
		printf("%ld %d %s\n",
			philo->state.current_time - philo->state.start_time, philo->number,
			str);
		result = sem_post(philo->mutex_dead);
		stop_routine(philo, result);
	}
}
