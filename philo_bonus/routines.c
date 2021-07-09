/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchopin <gchopin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/05 15:06:09 by gchopin           #+#    #+#             */
/*   Updated: 2021/07/06 20:58:13 by gchopin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher_bonus.h"

int	thinking(t_philosopher *philo)
{
	sem_wait(philo->secure);
	philo->state.current_time = math_time();
	//if (philo->dead == 0)
	//	printf("%ld %d is thinking\n",
	//		philo->state.current_time - philo->state.start_time, philo->number);
	if (philo->dead == 0)
	{
		display(philo, "is thinking", 0);
		philo->eat = 0;
		philo->sleep = 0;
	}
	sem_post(philo->secure);
	return (0);
}

int	sleeping(t_philosopher *philo)
{
	sem_wait(philo->secure);
	philo->state.current_time = math_time();
	if (philo->dead == 0)
	{
		display(philo, "is sleeping", 0);
		philo->sleep = 1;
	}
	//{
	//	printf("%ld %d is sleeping\n",
	//		philo->state.current_time - philo->state.start_time, philo->number);
	//}
	//else
	//	sem_wait(philo->mutex_dead);
	usleep(philo->state.time_to_sleep * 1000);
	sem_post(philo->secure);
	return (0);
}

int	eating(t_philosopher *philo)
{
	//sem_wait(philo->secure);
	philo->state.current_time = math_time();
	philo->state.time_simulation = math_time();
	if (philo->dead == 0)
	{
		display(philo, "is eating", 0);
		philo->nb_fork = 0;
		philo->eat = 1;
	}
	//if (philo->dead == 0)
	//	printf("%ld %d is eating\n",
	//		philo->state.current_time - philo->state.start_time, philo->number);
	usleep(philo->state.time_to_eat * 1000);
	if (philo->nb_time_active == 1)
	{
		philo->nb_time = philo->nb_time + 1;
		if (philo->nb_time >= philo->state.nb_time_eat
			&& philo->nb_time_reach == 0)
			philo->nb_time_reach = philo->nb_time_reach + 1;
	}
	//sem_post(philo->secure);
	sem_post(philo->fork);
	sem_post(philo->fork);
	return (0);
}

int	take_fork(t_philosopher *philo)
{
	int	result_one;
	int	result_two;

	result_one = sem_wait(philo->fork);
	result_two = sem_wait(philo->fork);
	philo->state.current_time = math_time();
	if (philo->dead == 0)
	{
		display(philo, "has taken a fork", 0);
		philo->nb_fork += 1;
	}
	//{
	//	sem_wait(philo->secure);
	//	printf("%ld %d has taken a fork left\n",
	//		philo->state.current_time - philo->state.start_time, philo->number);
	//	sem_post(philo->secure);
	//}
	philo->state.current_time = math_time();
	if (philo->dead == 0)
	{
		display(philo, "has taken a fork", 0);
		philo->nb_fork += 1;
	}
	//{
	//	sem_wait(philo->secure);
	//	printf("%ld %d has taken a fork right\n",
	//		philo->state.current_time - philo->state.start_time, philo->number);
	//	sem_post(philo->secure);
	//}
	return (result_one);
}
