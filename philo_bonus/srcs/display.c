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

int	display(t_philosopher *philo, char *str, int is_dead)
{
	int	result;

	result = 1;
	if (is_dead == 0)
	{
		result = sem_wait(philo->mutex_dead);
		if (result != 0)
			return (1);
		philo->state.current_time = math_time();
		if (philo->state.current_time == -1)
			return (1);
		stop_routine(philo, result);
		printf("%ld %d %s\n",
			philo->state.current_time - philo->state.start_time, philo->number,
			str);
		result = sem_post(philo->mutex_dead);
		stop_routine(philo, result);
	}
	return (0);
}
