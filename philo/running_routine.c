/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   running_routine.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchopin <gchopin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/17 18:46:37 by gchopin           #+#    #+#             */
/*   Updated: 2021/12/14 11:42:25 by gchopin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

static void	loop_routine(t_philosopher *philo, int *result)
{
	if (philo)
	{
		if (philo->nb_fork == 0 && *result == 0
			&& *philo->dead == 0 && philo->eat == 0
			&& philo->sleep == 0
			&& philo->nb_philosopher > *philo->everyone_eat)
			*result = take_fork(philo);
		if (philo->nb_fork == 2 && *result == 0
			&& *philo->dead == 0 && philo->eat == 0
			&& philo->sleep == 0
			&& philo->nb_philosopher > *philo->everyone_eat)
			*result = eating(philo);
		if (philo->nb_fork == 0 && *result == 0
			&& *philo->dead == 0 && philo->eat == 1
			&& philo->sleep == 0
			&& philo->nb_philosopher > *philo->everyone_eat)
			*result = sleeping(philo);
		if (philo->nb_fork == 0 && *result == 0
			&& *philo->dead == 0 && philo->eat == 1
			&& philo->sleep == 1
			&& philo->nb_philosopher > *philo->everyone_eat)
			*result = thinking(philo);
	}
}

static void	run_private_threads(t_philosopher *philosopher)
{
	pthread_create(&philosopher->thread, NULL,
		philo_is_dead, philosopher);
	pthread_detach(philosopher->thread);
	if (philosopher->nb_time_active == 1 && philosopher->nb_philosopher)
	{
		pthread_create(&philosopher->eat_thread, NULL,
			philo_is_eat, philosopher);
	}
}

void	*start_routine(void *args)
{
	t_philosopher	*philosopher;
	int				result;

	result = 0;
	philosopher = (t_philosopher *)args;
	if (philosopher)
	{
		run_private_threads(philosopher);
		while (philosopher && *philosopher->dead == 0 && result == 0
			&& philosopher->nb_philosopher > *philosopher->everyone_eat)
		{
			loop_routine(philosopher, &result);
		}
		if (philosopher->fork_right && philosopher->fork_right->fork_exist == 1)
			pthread_mutex_unlock(&philosopher->fork_right->mutex);
		if (philosopher->fork_left && philosopher->fork_left->fork_exist == 1)
			pthread_mutex_unlock(&philosopher->fork_left->mutex);
		if (philosopher->nb_time_active == 1 && philosopher->nb_philosopher)
			pthread_join(philosopher->eat_thread, NULL);
	}
	usleep(1000);
	return (NULL);
}
