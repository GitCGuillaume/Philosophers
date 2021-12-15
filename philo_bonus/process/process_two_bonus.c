/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_two_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchopin <gchopin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/17 17:16:05 by gchopin           #+#    #+#             */
/*   Updated: 2021/12/15 09:19:57 by gchopin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher_bonus.h"

void	clear_finish_eat(t_philosopher *philosopher)
{
	int	i;

	i = 0;
	if (!philosopher)
		return ;
	while (philosopher->nb_philosopher > i)
	{
		sem_post(philosopher->sem_eat_wait);
		i++;
	}
	if (pthread_join(philosopher->thread_wait_eat, NULL) != 0)
	{
		printf("Error\nCan't clear wait_eat_routine\n");
		sem_post(philosopher->wait_loop);
		kill(philosopher->process, SIGKILL);
	}
	if (philosopher->finish == 1 && philosopher->dead == 100)
	{
		sem_wait(philosopher->mutex_dead);
		printf("Everyone has eaten\n");
		sem_post(philosopher->wait_loop);
	}
}

void	loop_running_process(t_philosopher *philosopher)
{
	int	result;

	if (!philosopher)
		return ;
	result = 0;
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
}

int	run_process_two(t_philosopher *philo, long int current_time)
{
	if (philo->process == -1)
	{
		printf("Error returned\n");
		return (1);
	}
	else if (philo->process == 0)
	{
		philo->dead = 0;
		philo->state.start_time = current_time;
		philo->state.time_simulation = current_time;
		philo->state.current_time = current_time;
		if (pthread_create(&philo->thread, NULL,
				philo_dead_routine, philo) == 0)
		{
			start_routine(philo);
			sem_post(philo->wait_loop);
			exit(0);
		}
		sem_post(philo->wait_loop);
		exit(0);
	}
	return (0);
}
