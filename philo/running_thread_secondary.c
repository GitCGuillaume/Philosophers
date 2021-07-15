/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   running_thread_secondary.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchopin <gchopin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/15 11:42:29 by gchopin           #+#    #+#             */
/*   Updated: 2021/07/15 11:55:48 by gchopin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

void	*philo_eat_routine(void *args)
{
	t_philosopher	*philo;
	int				result;

	philo = (t_philosopher *)args;
	result = 0;
	pthread_mutex_lock(&philo->mutex_eat);
	while (result == 0 && *philo->dead == 0
			&& philo->nb_philosopher > *philo->everyone_eat)
	{
		if (result == 0 && philo->nb_time_reach > 0)
		{
			*philo->everyone_eat = *philo->everyone_eat + 1;
			result = 1;
		}
		usleep(10);
	}
	pthread_mutex_unlock(&philo->mutex_eat);
	return (NULL);
}

void	*philo_dead_routine(void *args)
{
	t_philosopher	*philo;
	int				result;

	philo = (t_philosopher *)args;
	result = 0;
	pthread_mutex_lock(&philo->mutex);
	while (result == 0 && *philo->dead == 0
		&& philo->nb_philosopher > *philo->everyone_eat)
	{
		result = is_dead(philo);
		if (result == 1)
		{
			return (NULL);
		}
		usleep(10);
	}
	pthread_mutex_unlock(&philo->mutex);
	return (NULL);
}
