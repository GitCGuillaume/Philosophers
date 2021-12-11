/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   running_process.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchopin <gchopin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/05 15:01:07 by gchopin           #+#    #+#             */
/*   Updated: 2021/12/10 18:30:49 by gchopin          ###   ########.fr       */
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
	t_philosopher	*philo;
	int	i;
	int	nb_eat;

	i = 0;
	nb_eat = 0;
	philo = (t_philosopher *)args;
	sem_wait(philo->sem_eat_finish);
	/*while (philo->nb_philosopher > nb_eat)
	{
		sem_wait(philo->sem_eat_finish);
		philo->dead = 1;
		nb_eat++;
		sem_wait(philo->mutex_dead);
	}
	printf("dead=%d\n", philo->dead);*/
	if (philo->dead == 0)
	{
		sem_wait(philo->mutex_dead);
		philo->dead = 100;
		philo->finish = 1;
		//printf("id=%d\ndead=%d\nfinish=%d\n", philo->number, philo->dead, philo->finish);
		//printf("Everyone has eaten\n");
		sem_post(philo->mutex_dead);
		return (NULL);
	}
	return (NULL);
}

void	*philo_dead_routine(void *args)
{
	t_philosopher	*philo;
	long int		current_time;
	int				result;
	int	i;

	i = 0;
	philo = (t_philosopher *)args;
	result = 0;
	while (philo->dead == 0 && philo->finish == 0)
	{
		//result = sem_wait(philo->mutex_dead);
		current_time = math_time();
		if (current_time > (philo->state.time_simulation
				+ philo->state.time_to_die)
			&& philo->dead == 0 && philo->finish == 0)
		{
			philo->dead = 1;
			usleep(800);
			sem_wait(philo->mutex_dead);
			current_time = math_time();
			if (philo->dead == 1)
				printf("%ld %d died\n",
					current_time
					- philo->state.start_time, philo->number);
			//sem_post(philo->mutex_dead);
			//	sem_post(philo->mutex_dead);
			//sem_post(philo->fork);
			//sem_post(philo->fork);
			while (philo->nb_philosopher >= i)
			{
				sem_post(philo->fork);
				i++;
			}
			if (philo->nb_time_active == 1 && philo->finish == 0)
			{
				//sem_post(philo->sem_eat_finish);
				/*while (philo->nb_philosopher >= i)
				{
					sem_post(philo->sem_eat_wait);
					i++;
				}*/
				i = 0;
			}
			//sem_post(philo->mutex_dead);
			return (NULL);
		}
		//result = sem_post(philo->mutex_dead);
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
		//pthread_detach(philosopher->thread_wait_eat);
	}
	return (result);
}

void	*start_routine(t_philosopher *philosopher)
{
	int			result;
	int	i;

	i = 0;
	if (!philosopher)
		exit(EXIT_FAILURE);
	result = launch_wait_thread(philosopher);
	while (philosopher->dead == 0) //&& philosopher->finish == 0)
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
	/*while (philosopher->nb_philosopher >= i)
	{
		sem_post(philosopher->fork);
		i++;
	}*/
	if (philosopher->nb_time_active == 1)
	{
		sem_post(philosopher->sem_eat_finish);
		if (pthread_join(philosopher->thread_wait_eat, NULL) != 0)
		{
			printf("Error\nCan't clear wait_eat_routine\n");
			sem_post(philosopher->wait_loop);
			exit(EXIT_FAILURE);
		}
		if (philosopher->finish == 1 && philosopher->dead == 100)
		{
			sem_wait(philosopher->mutex_dead);
			printf("Everyone has eaten\n");
		}
	}
	return (NULL);
}
