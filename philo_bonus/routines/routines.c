/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchopin <gchopin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/05 15:06:09 by gchopin           #+#    #+#             */
/*   Updated: 2021/12/10 18:25:39 by gchopin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher_bonus.h"

int	thinking(t_philosopher *philo)
{
	if (!philo)
		return (1);
	display(philo, "is thinking", philo->dead);
	philo->eat = 0;
	philo->sleep = 0;
	return (0);
}

int	sleeping(t_philosopher *philo)
{
	if (!philo)
		return (1);
	display(philo, "is sleeping", philo->dead);
	usleep(philo->state.time_to_sleep * 1000);
	philo->sleep = 1;
	return (0);
}

void	count_eat(t_philosopher *philo, int *result)
{
	if (!philo)
		return ;
	if (philo->nb_time_active == 1)
	{
		philo->nb_time = philo->nb_time + 1;
		if (philo->nb_time >= philo->state.nb_time_eat
			&& philo->nb_time_reach == 0)
		{
			philo->nb_time_reach = philo->nb_time_reach + 1;
			*result = sem_post(philo->sem_eat_wait);
			stop_routine(philo, *result);
		}
	}
}

int	eating(t_philosopher *philo)
{
	int	result;

	if (!philo)
		return (1);
	display(philo, "is eating", philo->dead);
	philo->state.time_simulation = math_time();
	usleep(philo->state.time_to_eat * 1000);
	count_eat(philo, &result);
	philo->nb_fork = 0;
	philo->eat = 1;
	result = sem_post(philo->fork);
	stop_routine(philo, result);
	result = sem_post(philo->fork);
	stop_routine(philo, result);
	return (result);
}

int	take_fork(t_philosopher *philo)
{
	int	result;

	if (!philo)
		return (1);
	if (philo->dead == 0)
	{
		result = sem_wait(philo->fork);
		stop_routine(philo, result);
		display(philo, "has taken a fork", philo->dead);
		philo->nb_fork += 1;
	}
	if (philo->dead == 0)
	{
		result = sem_wait(philo->fork);
		stop_routine(philo, result);
		display(philo, "has taken a fork", philo->dead);
		philo->nb_fork += 1;
	}
	return (result);
}
