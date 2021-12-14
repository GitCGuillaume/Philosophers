/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   running_process_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchopin <gchopin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/05 15:01:07 by gchopin           #+#    #+#             */
/*   Updated: 2021/12/14 14:23:44 by gchopin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher_bonus.h"

void	*philo_eat_routine(void *args)
{
	t_philosopher		*philo;
	int					nb_eat;

	nb_eat = 0;
	philo = (t_philosopher *)args;
	if (philo->nb_philosopher > 0)
	{
		while (philo->nb_philosopher > nb_eat)
		{
			if (sem_wait(philo->sem_eat_wait) != 0)
				kill(philo->process, SIGKILL);
			nb_eat++;
		}
		if (nb_eat == philo->nb_philosopher)
		{
			if (sem_post(philo->sem_eat_finish) != 0)
				kill(philo->process, SIGKILL);
			return (NULL);
		}
	}
	return (NULL);
}

void	*philo_wait_eat_routine(void *args)
{
	t_philosopher		*philo;

	philo = (t_philosopher *)args;
	if (sem_wait(philo->sem_eat_finish) != 0)
		philo->dead = 1;
	if (philo->dead == 0)
	{
		if (sem_wait(philo->mutex_dead) != 0)
			kill(philo->process, SIGKILL);
		philo->dead = 100;
		philo->finish = 1;
		if (sem_post(philo->mutex_dead) != 0)
			kill(philo->process, SIGKILL);
		return (NULL);
	}
	return (NULL);
}

void	*philo_dead_routine(void *args)
{
	t_philosopher		*philo;
	int					result;

	philo = (t_philosopher *)args;
	result = 0;
	while (philo->dead == 0 && philo->finish == 0
		&& result == 0)
	{
		if (sem_wait(philo->mutex_dead) != 0)
			kill(philo->process, SIGKILL);
		result = is_dead(philo);
		if (result)
		{
			return (NULL);
		}
		if (sem_post(philo->mutex_dead) != 0)
			kill(philo->process, SIGKILL);
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
			return (1);
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
	{
		sem_post(philosopher->wait_loop);
		return (NULL);
	}
	result = launch_wait_thread(philosopher);
	if (result)
		return (NULL);
	loop_running_process(philosopher);
	while (philosopher->nb_philosopher >= i)
	{
		sem_post(philosopher->fork);
		i++;
	}
	if (philosopher->nb_time_active == 1)
		clear_finish_eat(philosopher);
	sem_post(philosopher->wait_loop);
	return (NULL);
}
