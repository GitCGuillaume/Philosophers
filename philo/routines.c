/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchopin <gchopin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/02 11:46:21 by gchopin           #+#    #+#             */
/*   Updated: 2021/12/08 11:14:37 by gchopin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

int	thinking(t_philosopher *philo)
{
	int		result;

	result = 0;
	if (!philo)
		return (0);
	if (result == 0)
	{
		if (*philo->dead == 0)
		{
			pthread_mutex_lock(&philo->display);
			philo->state.current_time = math_time();
			if (*philo->dead == 0)
			{
				printf("%ld %d is thinking\n",
					philo->state.current_time - philo->state.start_time,
					philo->number);
			}
			philo->eat = 0;
			philo->sleep = 0;
				pthread_mutex_unlock(&philo->display);
		}
		else
			result = 1;
	}
	if (*philo->dead == 1)
		return (1);
	return (result);
}

int	sleeping(t_philosopher *philo)
{
	long int	current_time;
	int		result;

	result = 0;
	if (!philo)
		return (0);
	if (result == 0 && *philo->dead == 0)
	{
		pthread_mutex_lock(&philo->display);
		current_time = math_time();
		philo->state.current_time = current_time;
		if (*philo->dead == 0)
		{
			printf("%ld %d is sleeping\n",
				philo->state.current_time - philo->state.start_time,
				philo->number);
		}
		pthread_mutex_unlock(&philo->display);
		usleep(philo->state.time_to_sleep * 1000);
		philo->sleep = 1;
	}
	if (*philo->dead == 1)
		return (1);
	return (result);
}

int	eating(t_philosopher *philo)
{
	if (philo)
	{
		
		if (*philo->dead == 0)
		{
			pthread_mutex_lock(&philo->display);
			philo->state.current_time = math_time();
			philo->state.time_simulation = math_time();
			if (*philo->dead == 0)
				printf("%ld %d is eating\n",
					philo->state.current_time
					- philo->state.start_time, philo->number);
			pthread_mutex_unlock(&philo->display);
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
		if (philo->eat == 1)
			unlock_eating(philo);
	}
	if (*philo->dead == 1)
		return (1);
	return (0);
}

int	take_fork(t_philosopher *philo)
{
	int	result_one;

	result_one = 0;
	if (philo->fork_left && philo->fork_left->fork_exist == 1
		&& *philo->dead == 0)
	{
		result_one = pthread_mutex_lock(&philo->fork_left->mutex);
		if (*philo->dead == 0)
		{
			pthread_mutex_lock(&philo->display);
			philo->state.current_time = math_time();
			if (*philo->dead == 0)
				printf("%ld %d has taken a fork\n",
					philo->state.current_time - philo->state.start_time, philo->number);
			philo->nb_fork += 1;
			pthread_mutex_unlock(&philo->display);
		}
	}
	if (philo->fork_right && philo->fork_right->fork_exist == 1
		&& *philo->dead == 0)
	{
		result_one = pthread_mutex_lock(&philo->fork_right->mutex);
		if (*philo->dead == 0)
		{
			pthread_mutex_lock(&philo->display);
			philo->state.current_time = math_time();
			if (*philo->dead == 0)
				printf("%ld %d has taken a fork\n",
					philo->state.current_time - philo->state.start_time, philo->number);
			philo->nb_fork += 1;
			pthread_mutex_unlock(&philo->display);
		}
	}
	return (result_one);
}
