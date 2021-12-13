/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   running_process.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchopin <gchopin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/05 15:01:07 by gchopin           #+#    #+#             */
/*   Updated: 2021/12/13 11:12:00 by gchopin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher_bonus.h"

void	*philo_eat_routine(void *args)
{
	t_philosopher		*philo;
	int					nb_eat;
	int					i;

	nb_eat = 0;
	i = 0;
	philo = (t_philosopher *)args;
	if (philo->nb_philosopher > 0)
	{
		while (philo->nb_philosopher > nb_eat)
		{
			sem_wait(philo->sem_eat_wait);
			nb_eat++;
		}
		if (nb_eat == philo->nb_philosopher)
		{
			sem_post(philo->sem_eat_finish);
			return (NULL);
		}
	}
	return (NULL);
}

void	*philo_wait_eat_routine(void *args)
{
	t_philosopher		*philo;
	int					i;
	int					nb_eat;

	i = 0;
	nb_eat = 0;
	philo = (t_philosopher *)args;
	sem_wait(philo->sem_eat_finish);
	if (philo->dead == 0)
	{
		sem_wait(philo->mutex_dead);
		philo->dead = 100;
		philo->finish = 1;
		sem_post(philo->mutex_dead);
		return (NULL);
	}
	return (NULL);
}

void	*philo_dead_routine(void *args)
{
	t_philosopher		*philo;
	int					result;
	int					i;

	i = 0;
	philo = (t_philosopher *)args;
	result = 0;
	while (philo->dead == 0 && philo->finish == 0
		&& result == 0)
	{
		sem_wait(philo->mutex_dead);
		result = is_dead(philo);
		if (result)
		{
			return (NULL);
		}
		sem_post(philo->mutex_dead);
		usleep(200);
	}
	return (NULL);
}

int	launch_wait_thread(t_philosopher *philosopher)
{
	int				result;

	result = 0;
	if (philosopher->nb_philosopher > 0 && philosopher->nb_time_active == 1)
	{
		result = pthread_create(&philosopher->thread_wait_eat, NULL,
				philo_wait_eat_routine, philosopher);
		if (result != 0)
		{
			printf("Error\nCan't launch wait_eat_routine\n");
			sem_post(philosopher->wait_loop);
			exit(EXIT_FAILURE);
		}
	}
	return (result);
}

void	*start_routine(t_philosopher *philosopher)
{
	int	result;
	int	i;

	i = 0;
	if (!philosopher)
		exit(EXIT_FAILURE);
	result = launch_wait_thread(philosopher);
	loop_running_process(philosopher);
	while (philosopher->nb_philosopher >= i)
	{
		sem_post(philosopher->fork);
		i++;
	}
	if (philosopher->nb_time_active == 1)
		clear_finish_eat(philosopher);
	return (NULL);
}
