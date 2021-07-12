/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   running_thread.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchopin <gchopin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/02 11:32:34 by gchopin           #+#    #+#             */
/*   Updated: 2021/07/07 11:02:44 by gchopin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

static void	*philo_dead_routine(void *args)
{
	t_philosopher	*philo;
	int				result;

	philo = (t_philosopher *)args;
	result = 0;
	while (result == 0 && *philo->dead == 0)
	{
		pthread_mutex_lock(&philo->mutex);
		if (philo->nb_time_reach > 0)
		{
			pthread_mutex_unlock(&philo->mutex);
			return (NULL);
		}
		result = is_dead(philo);
		if (result == 1)
		{
			pthread_mutex_unlock(&philo->mutex);
			return (NULL);
		}
		pthread_mutex_unlock(&philo->mutex);
		usleep(5);
	}
	return (NULL);
}

static void	*start_routine(void *args)
{
	t_philosopher	*philosopher;
	int				result;

	result = 0;
	philosopher = (t_philosopher *)args;
	while (*philosopher->dead == 0)
	{
		if (philosopher->nb_fork == 0 && result == 0
			&& *philosopher->dead == 0 && philosopher->eat == 0
			&& philosopher->sleep == 0)
			result = take_fork(philosopher);
		if (philosopher->nb_fork == 2 && result == 0
			&& *philosopher->dead == 0 && philosopher->eat == 0
			&& philosopher->sleep == 0)
			result = eating(philosopher);
		if (philosopher->nb_fork == 0 && result == 0
			&& *philosopher->dead == 0 && philosopher->eat == 1
			&& philosopher->sleep == 0)
			result = sleeping(philosopher);
		if (philosopher->nb_fork == 0 && result == 0
			&& *philosopher->dead == 0 && philosopher->eat == 1
			&& philosopher->sleep == 1)
			result = thinking(philosopher);
	}
	return (NULL);
}

int	running_thread(t_philosopher **philosopher, int argc, int nb_philosopher)
{
	pthread_t	main_thread;
	int			i;
	int			dead;
	int			result;

	i = 0;
	dead = 0;
	result = 1;
	if (!philosopher)
		return (1);
	while (nb_philosopher > i)
	{
		philosopher[i]->dead = &dead;
		philosopher[i]->state.start_time = math_time();	
		result = pthread_create(&main_thread, NULL, start_routine, philosopher[i]);
		if (result != 0)
		{
			dead = 1;
			printf("Thread allocation failed.\n");
			return (1);
		}
		result = pthread_create(&philosopher[i]->thread, NULL,
			philo_dead_routine, philosopher[i]);
		if (result != 0)
		{
			dead = 1;
			result = pthread_join(main_thread, NULL);
			if (result != 0)
				printf("Thread terminated state failed.\n");
			printf("Thread allocation failed.\n");
			return (1);
		}
		result = pthread_join(main_thread, NULL);
		if (result != 0)
		{
			dead = 1;
			printf("Thread terminated state failed.\n");
			return (1);
		}
		i++;
		usleep(10);
	}
	i = 0;
	while (nb_philosopher > i)
	{
		pthread_join(philosopher[i]->thread, NULL);
		i++;
	}
	if (argc == 6 && dead == 0)
	{
		usleep(1);
		printf("Everyone has eaten\n");
	}
	return (0);
}
