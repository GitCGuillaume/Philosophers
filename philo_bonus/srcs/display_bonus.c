/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchopin <gchopin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/17 18:49:14 by gchopin           #+#    #+#             */
/*   Updated: 2021/12/14 16:50:05 by gchopin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher_bonus.h"

int	display(t_philosopher *philo, char *str, int is_dead)
{
	int	result;

	result = 1;
	if (!philo || !str)
		return (1);
	if (is_dead == 0)
	{
		result = sem_wait(philo->mutex_dead);
		if (result != 0)
			return (1);
		philo->state.current_time = math_time();
		if (philo->state.current_time == -1)
			return (1);
		printf("%ld %d %s\n",
			philo->state.current_time - philo->state.start_time, philo->number,
			str);
		if (sem_post(philo->mutex_dead) != 0)
			kill(philo->process, SIGKILL);
	}
	return (0);
}

int	display_eat(t_philosopher *philo, int is_dead)
{
	if (!philo)
		return (1);
	if (is_dead == 0)
	{
		if (sem_wait(philo->mutex_dead) != 0)
			return (1);
		philo->state.current_time = math_time();
		if (philo->state.current_time == -1)
			return (1);
		printf("%ld %d is eating\n", philo->state.current_time
			- philo->state.start_time, philo->number);
		philo->state.time_simulation = math_time();
		if (sem_post(philo->mutex_dead) != 0)
			kill(philo->process, SIGKILL);
	}
	return (0);
}
