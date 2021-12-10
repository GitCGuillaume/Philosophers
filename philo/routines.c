/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchopin <gchopin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/02 11:46:21 by gchopin           #+#    #+#             */
/*   Updated: 2021/12/10 09:14:16 by gchopin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

int	thinking(t_philosopher *philo)
{
	if (!philo)
		return (0);
	if (*philo->dead == 0)
	{
		if (pthread_mutex_lock(philo->display) != 0)
			return (1);
		philo->state.current_time = math_time();
		if (philo->state.current_time != -1
			&& *philo->dead == 0)
		{
			printf("%ld %d is thinking\n",
				philo->state.current_time - philo->state.start_time,
				philo->number);
		}
		pthread_mutex_unlock(philo->display);
		if (philo->state.current_time == -1)
			return (1);
		philo->eat = 0;
		philo->sleep = 0;
	}
	if (*philo->dead == 1)
		return (1);
	return (0);
}

int	sleeping(t_philosopher *philo)
{
	if (!philo)
		return (0);
	if (*philo->dead == 0)
	{
		if (pthread_mutex_lock(philo->display) != 0)
			return (1);
		philo->state.current_time = math_time();
		if (philo->state.current_time != -1
			&& *philo->dead == 0)
		{
			printf("%ld %d is sleeping\n",
				philo->state.current_time - philo->state.start_time,
				philo->number);
		}
		pthread_mutex_unlock(philo->display);
		if (philo->state.current_time == -1)
			return (1);
		usleep(philo->state.time_to_sleep * 1000);
		philo->sleep = 1;
	}
	if (*philo->dead == 1)
		return (1);
	return (0);
}

int	eating(t_philosopher *philo)
{
	if (!philo)
		return (1);
	if (*philo->dead == 0)
	{
		if (eating_two(philo))
		{
			unlock_eating(philo);
			return (1);
		}
		usleep(philo->state.time_to_eat * 1000);
		if (philo->nb_time_active == 1)
		{
			philo->nb_time = philo->nb_time + 1;
			if (philo->nb_time >= philo->state.nb_time_eat
				&& philo->nb_time_reach == 0)
				philo->nb_time_reach = philo->nb_time_reach + 1;
		}
		philo->eat = 1;
	}
	unlock_eating(philo);
	if (*philo->dead == 1)
		return (1);
	return (0);
}

int	take_fork(t_philosopher *philo)
{
	int	result;

	result = 0;
	if (!philo)
		return (1);
	result = take_fork_left(philo);
	if (result)
		return (1);
	result = take_fork_right(philo);
	if (result)
		return (1);
	return (0);
}
