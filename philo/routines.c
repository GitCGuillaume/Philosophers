/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchopin <gchopin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/02 11:46:21 by gchopin           #+#    #+#             */
/*   Updated: 2021/07/06 20:59:35 by gchopin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

int	thinking(t_philosopher *philo)
{
	pthread_mutex_lock(&philo->secure);
	philo->state.current_time = math_time();
	if (*philo->dead == 0)
	{
		printf("%ld %d is thinking\n",
			philo->state.current_time - philo->state.start_time, philo->number);
		philo->eat = 0;
		philo->sleep = 0;
	}	
	pthread_mutex_unlock(&philo->secure);
	return (0);
}

int	sleeping(t_philosopher *philo)
{
	pthread_mutex_lock(&philo->secure);
	philo->state.current_time = math_time();
	if (*philo->dead == 0)
	{
		philo->sleep = 1;
		printf("%ld %d is sleeping\n",
			philo->state.current_time - philo->state.start_time, philo->number);
		usleep(philo->state.time_to_sleep * 1000);
	}	
	pthread_mutex_unlock(&philo->secure);
	return (0);
}

int	eating(t_philosopher *philo)
{
	philo->state.current_time = math_time();
	philo->state.time_simulation = math_time();
	if (*philo->dead == 0)
	{
		printf("%ld %d is eating\n",
			philo->state.current_time - philo->state.start_time, philo->number);
		usleep(philo->state.time_to_eat * 1000);
	}	
	if (philo->nb_time_active == 1)
	{
		philo->nb_time = philo->nb_time + 1;
		if (philo->nb_time >= philo->state.nb_time_eat
			&& philo->nb_time_reach == 0)
			philo->nb_time_reach = philo->nb_time_reach + 1;
	}
	philo->nb_fork = 0;
	philo->eat = 1;
	pthread_mutex_unlock(&philo->fork_right->mutex);
	pthread_mutex_unlock(&philo->fork_left->mutex);
	return (0);
}

int	take_fork(t_philosopher *philo)
{
	int	result_one;
	int	result_two;

	result_one = pthread_mutex_lock(&philo->fork_left->mutex);
	philo->state.current_time = math_time();
	if (*philo->dead == 0)
	{
		printf("%ld %d has taken a fork\n",
			philo->state.current_time - philo->state.start_time, philo->number);
		philo->nb_fork += 1;
	}
	result_two = pthread_mutex_lock(&philo->fork_right->mutex);
	philo->state.current_time = math_time();
	if (*philo->dead == 0)
	{
		printf("%ld %d has taken a fork\n",
			philo->state.current_time - philo->state.start_time, philo->number);
		philo->nb_fork += 1;
	}
	return (result_one);
}
