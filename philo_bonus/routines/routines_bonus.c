/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchopin <gchopin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/05 15:06:09 by gchopin           #+#    #+#             */
/*   Updated: 2021/12/14 15:41:45 by gchopin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher_bonus.h"

int	thinking(t_philosopher *philo)
{
	if (!philo)
		return (1);
	if (display(philo, "is thinking", philo->dead) != 0)
		return (1);
	philo->eat = 0;
	philo->sleep = 0;
	return (0);
}

int	sleeping(t_philosopher *philo)
{
	if (!philo)
		return (1);
	if (display(philo, "is sleeping", philo->dead) != 0)
		return (1);
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
		}
	}
}

int	eating(t_philosopher *philo)
{
	int	result;

	result = 0;
	if (!philo)
		return (1);
	if (display(philo, "is eating", philo->dead) != 0)
		return (1);
	philo->state.time_simulation = math_time();
	if (philo->state.time_simulation == -1)
		return (1);
	usleep(philo->state.time_to_eat * 1000);
	count_eat(philo, &result);
	if (result != 0)
		return (1);
	philo->nb_fork = 0;
	philo->eat = 1;
	result = sem_post(philo->fork);
	if (result != 0)
		return (1);
	result = sem_post(philo->fork);
	if (result != 0)
		return (1);
	return (result);
}

int	take_fork(t_philosopher *philo)
{
	if (!philo)
		return (1);
	if (philo->dead == 0)
	{
		if (sem_wait(philo->fork) != 0)
			return (1);
		if (display(philo, "has taken a fork", philo->dead) != 0)
			return (1);
		philo->nb_fork += 1;
	}
	if (philo->dead == 0)
	{
		if (sem_wait(philo->fork) != 0)
			return (1);
		if (display(philo, "has taken a fork", philo->dead) != 0)
			return (1);
		philo->nb_fork += 1;
	}
	return (0);
}
