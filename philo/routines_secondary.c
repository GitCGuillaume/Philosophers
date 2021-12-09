/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines_secondary.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchopin <gchopin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/16 11:24:24 by gchopin           #+#    #+#             */
/*   Updated: 2021/12/08 10:27:34 by gchopin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

int	unlock_eating(t_philosopher *philo)
{
	int	result;

	result = 0;
	if (philo->fork_right && philo->fork_right->fork_exist == 1
		&& philo->nb_fork > 0)
	{
		result = pthread_mutex_unlock(&philo->fork_right->mutex);
		philo->nb_fork = philo->nb_fork - 1;
	}
	if (result != 0)
		printf("Error unlock mutex\n");
	if (philo->fork_left && philo->fork_left->fork_exist == 1
		&& philo->nb_fork > 0)
	{
		result = pthread_mutex_unlock(&philo->fork_left->mutex);
		philo->nb_fork = philo->nb_fork - 1;
	}
	if (result != 0)
		printf("Error unlock mutex\n");
	return (result);
}

int	eating_two(t_philosopher *philo)
{
	if (!philo)
		return (1);
	if (pthread_mutex_lock(philo->display) != 0)
		return (1);
	philo->state.current_time = math_time();
	if (philo->state.current_time == -1
		|| philo->state.time_simulation == -1)
	{
		pthread_mutex_unlock(philo->display);
		return (1);
	}
	if (*philo->dead == 0)
	{
		printf("%ld %d is eating\n",
			philo->state.current_time
			- philo->state.start_time, philo->number);
	}
	philo->state.time_simulation = math_time();
	pthread_mutex_unlock(philo->display);
	return (0);
}

int	take_fork_left(t_philosopher *philo)
{
	if (!philo)
		return (1);
	if (philo->fork_left && philo->fork_left->fork_exist == 1
		&& *philo->dead == 0)
	{
		if (pthread_mutex_lock(&philo->fork_left->mutex) != 0)
			return (0);
		if (*philo->dead == 0)
		{
			if (pthread_mutex_lock(philo->display) != 0)
				return (1);
			philo->state.current_time = math_time();
			if (philo->state.current_time != -1
				&& *philo->dead == 0)
				printf("%ld %d has taken a fork\n",
						philo->state.current_time
				- philo->state.start_time, philo->number);
			philo->nb_fork += 1;
			pthread_mutex_unlock(philo->display);
		}
	}
	if (*philo->dead == 1)
		return (1);
	return (0);
}

int	take_fork_right(t_philosopher *philo)
{
	if (!philo)
		return (1);
	if (philo->fork_right && philo->fork_right->fork_exist == 1
		&& *philo->dead == 0)
	{
		if (pthread_mutex_lock(&philo->fork_right->mutex) != 0)
			return (1);
		if (*philo->dead == 0)
		{
			if (pthread_mutex_lock(philo->display) != 0)
				return (1);
			philo->state.current_time = math_time();
			if (philo->state.current_time != -1
				&& *philo->dead == 0)
				printf("%ld %d has taken a fork\n",
					philo->state.current_time
					- philo->state.start_time, philo->number);
			philo->nb_fork += 1;
			pthread_mutex_unlock(philo->display);
		}
	}
	if (*philo->dead == 1)
		return (1);
	return (0);
}
