/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads_tools.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchopin <gchopin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/02 13:36:21 by gchopin           #+#    #+#             */
/*   Updated: 2021/07/07 14:28:28 by gchopin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher_bonus.h"

long int	math_time(void)
{
	struct timeval	time;
	int				res;
	long int				value;

	res = gettimeofday(&time, NULL);
	if (res == 0)
		value = (time.tv_sec * 1000) + (time.tv_usec / 1000);
	else
		return (-1);
	return (value);
}

int	is_dying(long int current_time, long int time_simulation, long int time_to)
{
	if (current_time > (time_simulation + time_to))
	{
		return (1);
	}
	return (0);
}

int	is_dead(t_philosopher *philo)
{
	long int	current_time;
	//int			result;

	current_time = math_time();
	//result = is_dying(current_time, philo->state.time_simulation,
	//		philo->state.time_to_die);
	//if (result == 1)
	//{
	if (current_time > (philo->state.time_simulation + philo->state.time_to_die))
	{
		//current_time = math_time();
		*philo->dead = 1;
		printf("%ld %d died\n",
			current_time - philo->state.start_time, philo->number);
		return (1);
	}
	return (0);
}
