/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchopin <gchopin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/05 15:06:09 by gchopin           #+#    #+#             */
/*   Updated: 2021/07/15 15:40:25 by gchopin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher_bonus.h"

void	stop_routine(t_philosopher *philo, int result)
{
	if (result != 0)
	{
		philo->dead = 1;
		sem_post(philo->wait_loop);
		exit(3);
	}
}

void	stop_routine_timer(t_philosopher *philo)
{
	if (philo->state.current_time == -1)
	{
		philo->dead = 1;
		sem_post(philo->wait_loop);
		exit(3);
	}
}

void	stop_routine_timer_sim(t_philosopher *philo)
{
	if (philo->state.time_simulation == -1)
	{
		philo->dead = 1;
		sem_post(philo->wait_loop);
		exit(3);
	}
}

int	thinking(t_philosopher *philo)
{
	int	result;

	result = sem_wait(philo->secure);
	stop_routine(philo, result);
	philo->state.current_time = math_time();
	stop_routine_timer(philo);
	display(philo, "is thinking", 0);
	philo->eat = 0;
	philo->sleep = 0;
	result = sem_post(philo->secure);
	stop_routine(philo, result);
	return (0);
}

int	sleeping(t_philosopher *philo)
{
	int	result;

	result = sem_wait(philo->secure);	
	stop_routine(philo, result);
	philo->state.current_time = math_time();
	stop_routine_timer(philo);
	display(philo, "is sleeping", 0);
	philo->sleep = 1;
	result = sem_post(philo->secure);
	stop_routine(philo, result);
	usleep(philo->state.time_to_sleep * 1000);
	return (0);
}

int	eating(t_philosopher *philo)
{
	int	result;

	philo->state.current_time = math_time();
	stop_routine_timer(philo);
	philo->state.time_simulation = math_time();
	stop_routine_timer_sim(philo);
	display(philo, "is eating", 0);
	philo->nb_fork = 0;
	philo->eat = 1;
	if (philo->nb_time_active == 1)
	{
		philo->nb_time = philo->nb_time + 1;
		if (philo->nb_time >= philo->state.nb_time_eat
			&& philo->nb_time_reach == 0)
		{
			philo->nb_time_reach = philo->nb_time_reach + 1;
			result = sem_post(philo->sem_eat_wait);
			stop_routine(philo, result);
		}
	}
	usleep(philo->state.time_to_eat * 1000);
	result = sem_post(philo->fork);
	stop_routine(philo, result);
	result = sem_post(philo->fork);
	stop_routine(philo, result);
	return (0);
}

int	take_fork(t_philosopher *philo)
{
	int	result;

	result = sem_wait(philo->fork);
	stop_routine(philo, result);
	result = sem_wait(philo->fork);
	stop_routine(philo, result);
	philo->state.current_time = math_time();
	stop_routine_timer(philo);
	if (philo->dead == 0)
	{
		display(philo, "has taken a fork", 0);
		philo->nb_fork += 1;
	}
	if (philo->dead == 0)
	{
		display(philo, "has taken a fork", 0);
		philo->nb_fork += 1;
	}
	return (result);
}
