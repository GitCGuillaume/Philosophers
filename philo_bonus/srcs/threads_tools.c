/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads_tools.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchopin <gchopin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/02 13:36:21 by gchopin           #+#    #+#             */
/*   Updated: 2021/07/17 18:04:41 by gchopin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher_bonus.h"

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
	if (current_time > (philo->state.time_simulation
				+ philo->state.time_to_die)
			&& philo->finish == 0 && philo->dead == 0)
	{
		philo->dead = 1;
		current_time = math_time();
		printf("%ld %d died\n",
			current_time
				- philo->state.start_time, philo->number);
		sem_post(philo->wait_loop);
		return (1);
	}
	return (0);
}
