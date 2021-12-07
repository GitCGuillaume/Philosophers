/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchopin <gchopin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/02 11:46:21 by gchopin           #+#    #+#             */
/*   Updated: 2021/12/07 15:44:39 by gchopin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

int	thinking(t_philosopher *philo)
{
	int		result;

	if (!philo)
		return (0);
	result = pthread_mutex_lock(&philo->secure);
	if (result == 0)
	{
		pthread_mutex_lock(&philo->display);
		if (*philo->dead == 0 && philo->state.current_time != -1)
		{
			
			philo->state.current_time = math_time();
			printf("%ld %d is thinking\n",
				philo->state.current_time - philo->state.start_time,
				philo->number);
			philo->eat = 0;
			philo->sleep = 0;
			pthread_mutex_unlock(&philo->display);
		}
		else
			result = 1;
		if (*philo->dead == 1)
			pthread_mutex_unlock(&philo->display);
		if (result == 1)
			pthread_mutex_unlock(&philo->secure);
		else
			result = pthread_mutex_unlock(&philo->secure);
	}
	return (result);
}

int	sleeping(t_philosopher *philo)
{
	long int	current_time;
	int		result;

	result = 1;
	if (!philo)
		return (0);
	result = pthread_mutex_lock(&philo->secure);
	if (result == 0) //&& current_time != -1)
	{
		pthread_mutex_lock(&philo->display);
		if (*philo->dead == 0) //&& philo->state.current_time != -1)
		{
			current_time = math_time();
			philo->state.current_time = current_time;
			philo->sleep = 1;
			printf("%ld %d is sleeping\n",
				philo->state.current_time - philo->state.start_time,
				philo->number);
			pthread_mutex_unlock(&philo->display);
			usleep(philo->state.time_to_sleep * 1000);
			philo->state.time_simulation = philo->state.current_time;
		}
		if (*philo->dead == 1)
			pthread_mutex_unlock(&philo->display);
		if (philo->state.current_time == -1)
		{
			result = 1;
			pthread_mutex_unlock(&philo->secure);
		}
		else
			result = pthread_mutex_unlock(&philo->secure);
	}
	return (result);
}

int	eating(t_philosopher *philo)
{
	if (philo)
	{
		pthread_mutex_lock(&philo->display);
		philo->state.current_time = math_time();
		if (/*philo->dead == 0 &&*/ philo->state.time_simulation != -1)
		{
			philo->state.time_simulation = math_time();
			printf("%ld %d is eating\n",
				philo->state.current_time
				- philo->state.start_time, philo->number);
			pthread_mutex_unlock(&philo->display);
			usleep(philo->state.time_to_eat * 1000);
			if (philo->nb_time_active == 1)
			{
				philo->nb_time = philo->nb_time + 1;
				if (philo->nb_time >= philo->state.nb_time_eat
					&& philo->nb_time_reach == 0)
					philo->nb_time_reach = philo->nb_time_reach + 1;
			}
			philo->eat = 1;
		}
		if (*philo->dead == 1)
			pthread_mutex_unlock(&philo->display);
		philo->nb_fork = 0;
		unlock_eating(philo);
	}
	return (0);
}

int	take_fork(t_philosopher *philo)
{
	int	result_one;

	result_one = 0;
	//if (philo && philo->fork_left && *philo->dead == 0
	//	&& philo->fork_left->fork_exist == 1)
	//{
		result_one = pthread_mutex_lock(&philo->fork_left->mutex);
		//pthread_mutex_lock(&philo->display);
		philo->nb_fork += 1;
		pthread_mutex_lock(&philo->display);
		if (*philo->dead == 0)
		{
			philo->state.current_time = math_time();
			printf("%ld %d has taken a fork\n",
				philo->state.current_time - philo->state.start_time, philo->number);
		}
		pthread_mutex_unlock(&philo->display);
		result_one = pthread_mutex_lock(&philo->fork_right->mutex);
		//pthread_mutex_lock(&philo->display);
		philo->nb_fork += 1;
		pthread_mutex_lock(&philo->display);
		if (*philo->dead == 0)
		{
			philo->state.current_time = math_time();
			printf("%ld %d has taken a fork\n",
				philo->state.current_time - philo->state.start_time, philo->number);
		}
		pthread_mutex_unlock(&philo->display);
		//pthread_mutex_unlock(&philo->display);
	//}
	//if (philo && /**philo->dead == 0 && result_one == 0
	//	&&*/ philo->state.current_time != -1)
	//{
	//	result_one = take_fork_two(philo);
	//}
	/*else if (result_one == 0 && philo && philo->fork_left)
	{
		pthread_mutex_unlock(&philo->fork_left->mutex);
		return (1);
	}*/
	return (result_one);
}
