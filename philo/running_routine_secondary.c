/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   running_routine_secondary.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchopin <gchopin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/15 11:42:29 by gchopin           #+#    #+#             */
/*   Updated: 2021/12/14 11:39:45 by gchopin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

void	*philo_is_eat(void *args)
{
	t_philosopher	*philo;
	int				result;

	philo = (t_philosopher *)args;
	result = 0;
	while (result == 0 && *philo->dead == 0
		&& philo->nb_philosopher > *philo->everyone_eat)
	{
		if (result == 0 && philo->nb_time_reach > 0)
		{
			*philo->everyone_eat = *philo->everyone_eat + 1;
			result = 1;
			return (NULL);
		}
		usleep(300);
	}
	return (NULL);
}

void	*philo_is_dead(void *args)
{
	t_philosopher	*philo;
	int				result;

	philo = (t_philosopher *)args;
	result = 0;
	while (result == 0 && *philo->dead == 0
		&& philo->nb_philosopher > *philo->everyone_eat)
	{
		pthread_mutex_lock(philo->display);
		result = is_dead(philo);
		if (result == 1)
		{
			pthread_mutex_unlock(philo->display);
			if (philo->fork_right && philo->fork_right->fork_exist == 1)
				pthread_mutex_unlock(&philo->fork_right->mutex);
			if (philo->fork_left && philo->fork_left->fork_exist == 1)
				pthread_mutex_unlock(&philo->fork_left->mutex);
			return (NULL);
		}
		pthread_mutex_unlock(philo->display);
		usleep(200);
	}
	return (NULL);
}
