/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchopin <gchopin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/17 17:08:50 by gchopin           #+#    #+#             */
/*   Updated: 2021/12/02 13:50:59 by gchopin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher_bonus.h"

long int	start_process(sem_t **wait_loop, t_philosopher *philo)
{
	long int	current_time;

	current_time = math_time();
	if (current_time == -1)
		return (-1);
	if (wait_loop)
		*wait_loop = sem_open("wait_loop", O_CREAT, S_IRWXU, 1);
	else
		return (-1);
	if (*wait_loop == SEM_FAILED)
		return (-1);
	sem_unlink("wait_loop");
	philo->wait_loop_exist = 1;
	return (current_time);
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
			//pthread_join(philo->thread_eat, NULL);
			//anti valgrind  pthreadleak
			usleep(800);
		}
	}
}

/*
 ** loop to run process
*/

int	loop_process(t_philosopher **philo, sem_t *wait_loop,
	long int current_time, int nb_philosopher)
{
	int	i;

	i = 0;
	if (philo)
	{
		while (nb_philosopher > i)
		{
			philo[i]->wait_loop = wait_loop;
			philo[i]->process = fork();
			if (run_process_two(philo, philo[i], current_time) == 1)
			{
				loop_process_two(philo, nb_philosopher);
				sem_post(wait_loop);
				usleep(800);
				return (2);
			}
			i++;
			usleep(1000);
		}
	}
	return (0);
}

int	run_process(t_philosopher **philo, int nb_philosopher)
{
	sem_t		*wait_loop;
	long int	current_time;
	int	i;

	i = 0;
	if (!philo)
		return (2);
	current_time = start_process(&wait_loop, philo[0]);
	if (current_time == -1)
		return (2);
	philo[0]->wait_loop = wait_loop;
	if (start_eat_thread(philo[0], wait_loop) != 0)
	{
		sem_close(wait_loop);
		return (2);
	}
	if (loop_process(philo, wait_loop, current_time, nb_philosopher) == 2)
		return (2);
	waitpid(0, 0, 0);
	/*while (waitpid(philo[i]->process, 0, WNOHANG))
	{
		i++;
		if (i == nb_philosopher)
			i = 0;
	}*/
	//waitpiddd();
	//sem_wait(wait_loop);
	end_process(philo[0], nb_philosopher);
	return (0);
}
