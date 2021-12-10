/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   running_process.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchopin <gchopin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/05 15:01:07 by gchopin           #+#    #+#             */
/*   Updated: 2021/12/10 18:25:07 by gchopin          ###   ########.fr       */
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
			usleep(10);
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
	t_philosopher	*philo;

	philo = (t_philosopher *)args;
	sem_wait(philo->sem_eat_finish);
	if (philo->dead == 0)
		display(philo, "Everyone has eaten", 0);
	sem_post(philo->wait_loop);
	exit(EXIT_SUCCESS);
	return (NULL);
}

void	*philo_dead_routine(void *args)
{
	t_philosopher	*philo;
	long int		current_time;
	int				result;

	philo = (t_philosopher *)args;
	result = 0;
	current_time = math_time();
	while (philo->dead == 0)
	{
		current_time = math_time();
		if (current_time > (philo->state.time_simulation
				+ philo->state.time_to_die)
			&& philo->dead == 0)
		{
			result = sem_wait(philo->mutex_dead);
			display(philo, "died", 1);
			philo->dead = 1;
			exit(EXIT_SUCCESS);
			return (NULL);
		}
		usleep(100);
	}
	return (NULL);
}

int	launch_thread(t_philosopher *philosopher)
{
	pthread_t		thread_eat;
	int				result;

	result = 0;
	if (philosopher->nb_philosopher > 0 && philosopher->nb_time_active == 1)
	{
		result = pthread_create(&thread_eat, NULL,
				philo_wait_eat_routine, philosopher);
		if (result != 0)
		{
			printf("Error\nCan't launch wait_eat_routine\n");
			sem_post(philosopher->wait_loop);
			exit(EXIT_FAILURE);
		}
		result = pthread_detach(thread_eat);
		if (result != 0)
		{
			printf("Error\nCan't clear wait_eat_routine\n");
			sem_post(philosopher->wait_loop);
			exit(EXIT_FAILURE);
		}
	}
	return (result);
}

void	*start_routine(t_philosopher *philosopher)
{
	int			result;

	result = 0;
	result = launch_thread(philosopher);
	while (philosopher->dead == 0)
	{	
		if (philosopher->nb_fork == 0 && result == 0
			&& philosopher->eat == 0 && philosopher->dead == 0
			&& philosopher->sleep == 0)
			result = take_fork(philosopher);
		if (philosopher->nb_fork == 2 && result == 0
			&& philosopher->eat == 0 && philosopher->dead == 0
			&& philosopher->sleep == 0)
			result = eating(philosopher);
		if (philosopher->nb_fork == 0 && result == 0
			&& philosopher->eat == 1 && philosopher->dead == 0
			&& philosopher->sleep == 0)
			result = sleeping(philosopher);
		if (philosopher->nb_fork == 0 && result == 0
			&& philosopher->eat == 1 && philosopher->dead == 0
			&& philosopher->sleep == 1)
			result = thinking(philosopher);
	}
	return (NULL);
}
