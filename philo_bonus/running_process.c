/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   running_process.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchopin <gchopin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/05 15:01:07 by gchopin           #+#    #+#             */
/*   Updated: 2021/07/07 14:59:11 by gchopin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher_bonus.h"

void	*philo_dead_routine(void *args)
{
	t_philosopher	*philo;
	int				result;

	philo = (t_philosopher *)args;
	result = 0;
	while (result == 0 && *philo->dead == 0)
	{
		sem_wait(philo->mutex);
		if (philo->nb_time_reach > 0)
		{
		//	kill(philo->process, SIGKILL);
			sem_post(philo->mutex);
			return (NULL);
		}
		result = is_dead(philo);
		if (result == 1)
		{
			*philo->dead = 1;
			kill(philo->process, SIGKILL);
			sem_post(philo->mutex);
			return (NULL);
		}
		sem_post(philo->mutex);
		usleep(5);
	}
	return (NULL);
}

void	*start_routine(t_philosopher *philosopher)
{
	int				result;
	int	i;

	result = 0;
	i = 0;
	/*sem_wait(philosopher->fork);
	printf("Hello\n");
	sem_post(philosopher->fork);
	*/
	/*while (1)
	{
		printf("i=%d dead=%d\n", i, *philosopher->dead);
		i++;
		}*/
	pthread_create(&philosopher->thread, NULL,
		philo_dead_routine, philosopher);
	pthread_detach(philosopher->thread);
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
