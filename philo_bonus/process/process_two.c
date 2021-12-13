/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_two.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchopin <gchopin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/17 17:16:05 by gchopin           #+#    #+#             */
/*   Updated: 2021/12/13 11:13:01 by gchopin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher_bonus.h"

int	run_process_two(t_philosopher *philo, long int current_time)
{
	int	i;
	int	nb_philosophers;

	i = 0;
	nb_philosophers = philo->nb_philosopher;
	if (philo->process == -1)
	{
		printf("Error no process returned\n");
		return (1);
	}
	else if (philo->process == 0)
	{
		sem_unlink("dead");
		sem_unlink("mutex1");
		sem_unlink("name");
		if (!philo)
			exit(EXIT_FAILURE);
		philo->dead = 0;
		philo->state.start_time = current_time;
		philo->state.time_simulation = current_time;
		philo->state.current_time = current_time;
		if (pthread_create(&philo->thread, NULL,
				philo_dead_routine, philo) != 0)
			exit(EXIT_FAILURE);
		pthread_detach(philo->thread);
		start_routine(philo);
		exit(EXIT_SUCCESS);
	}
	return (0);
}

int	start_eat_thread(t_philosopher *philo, sem_t *wait_loop)
{
	pthread_t		thread_eat;
	int				result;

	if (philo)
	{
		if (philo->nb_time_active == 1)
		{
			result = pthread_create(&thread_eat,
					NULL, philo_eat_routine, philo);
			if (result != 0)
			{
				philo->wait_loop = wait_loop;
				return (2);
			}
			pthread_detach(thread_eat);
		}
	}
	return (0);
}
