/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads_tools.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchopin <gchopin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/02 13:36:21 by gchopin           #+#    #+#             */
/*   Updated: 2021/12/08 11:43:04 by gchopin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

long int	math_time(void)
{
	struct timeval	time;
	int				res;
	long int		value;

	res = gettimeofday(&time, NULL);
	if (res == 0)
		value = (time.tv_sec * 1000) + (time.tv_usec / 1000);
	else
		return (-1);
	return (value);
}

int	is_dead(t_philosopher *philo)
{
	long int	current_time;

	current_time = math_time();
	//pthread_mutex_lock(&philo->display);
	if (current_time != -1 && *philo->dead == 0
		&& current_time > (philo->state.time_simulation
			+ philo->state.time_to_die))
	{
		//pthread_mutex_lock(&philo->display);
		*philo->dead = 1;
		printf("%ld %d died\n",
			current_time - philo->state.start_time, philo->number);
		//pthread_mutex_unlock(&philo->display);
		return (1);
	}
	//pthread_mutex_unlock(&philo->display);
	return (0);
}
