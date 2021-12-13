/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchopin <gchopin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/17 17:08:50 by gchopin           #+#    #+#             */
/*   Updated: 2021/12/13 12:56:56 by gchopin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher_bonus.h"

long int	start_process(sem_t **wait_loop, t_philosopher *philo)
{
	int	result;

	result = 1;
	if (!wait_loop)
		return (-1);
	*wait_loop = sem_open("wait_loop", O_CREAT, S_IRWXU, 0);
	if (*wait_loop == SEM_FAILED)
		return (-1);
	sem_unlink("wait_loop");
	if (philo->nb_time_active == 1)
	{
		result = pthread_create(&philo->thread_eat,
				NULL, philo_eat_routine, philo);
		if (result != 0)
		{
			sem_close(*wait_loop);
			philo->wait_loop = *wait_loop;
			return (2);
		}
	}
	return (0);
}

void	loop_process_two(t_philosopher **philo, int nb_philosopher)
{
	int	i;

	i = 0;
	if (philo)
	{
		while (philo[0]->nb_time_active == 1 && nb_philosopher > i)
		{
			sem_post(philo[0]->sem_eat_wait);
			sem_post(philo[0]->sem_eat_finish);
			i++;
			usleep(10);
		}
		i = 0;
		while (nb_philosopher > i)
		{
			if (philo[i]->process != -1)
				kill(philo[i]->process, SIGKILL);
			i++;
		}
	}
}

/*
 ** Will check if number time is allowed
 ** if allowed will loop to unlock wait eat semaphore
*/

void	end_process(t_philosopher *philo, int nb_philosopher)
{
	int	i;

	i = 0;
	if (philo)
	{
		if (philo->nb_time_active == 1)
		{
			while (nb_philosopher > i)
			{
				sem_post(philo->sem_eat_wait);
				i++;
				usleep(10);
			}
			pthread_join(philo->thread_eat, NULL);
			usleep(1000);
		}
	}
}

/*
 ** loop to run process
*/

int	loop_process(t_philosopher **philo, sem_t *wait_loop, int nb_philosopher)
{
	long int	current_time;
	int			i;

	i = 0;
	if (!philo)
		return (2);
	current_time = math_time();
	if (current_time != -1)
	{
		while (nb_philosopher > i)
		{
			philo[i]->wait_loop = wait_loop;
			philo[i]->process = fork();
			if (run_process_two(philo[i], current_time) == 1)
			{
				loop_process_two(philo, nb_philosopher);
				usleep(800);
				return (2);
			}
			i++;
			usleep(100);
		}
	}
	return (0);
}

int	run_process(t_philosopher **philo, int nb_philosopher)
{
	sem_t		*wait_loop;
	int			i;

	i = 0;
	if (!philo)
		return (2);
	if (start_process(&wait_loop, philo[0]) == -1)
		return (2);
	/*if (start_eat_thread(philo[0], wait_loop) != 0)
	{
		sem_close(wait_loop);
		return (2);
	}*/
	if (loop_process(philo, wait_loop, nb_philosopher) == 2)
	{
		sem_close(wait_loop);
		return (2);
	}
	sem_wait(wait_loop);
	while (nb_philosopher > i)
	{
		kill(philo[i]->process, SIGKILL);
		i++;
	}
	waitpid(0, 0, 0);
	end_process(philo[0], nb_philosopher);
	return (0);
}
