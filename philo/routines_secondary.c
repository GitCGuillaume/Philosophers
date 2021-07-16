/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_secondary.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchopin <gchopin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/16 11:24:24 by gchopin           #+#    #+#             */
/*   Updated: 2021/07/16 11:33:02 by gchopin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

int	unlock_eating(t_philosopher *philo)
{
	int	result;

	result = pthread_mutex_unlock(&philo->fork_right->mutex);
	if (result != 0)
		printf("Error unlock mutex\n");
	result = pthread_mutex_unlock(&philo->fork_left->mutex);
	if (result != 0)
		printf("Error unlock mutex\n");
	return (result);
}

int	take_fork_two(t_philosopher *philo)
{
	int	result_two;

	result_two = 0;
	printf("%ld %d has taken a fork\n",
		philo->state.current_time - philo->state.start_time, philo->number);
	philo->state.current_time = math_time();
	result_two = pthread_mutex_lock(&philo->fork_right->mutex);
	philo->nb_fork += 1;
	if (*philo->dead == 0 && result_two == 0 && philo->state.current_time != -1)
	{
		printf("%ld %d has taken a fork\n",
			philo->state.current_time - philo->state.start_time, philo->number);
	}
	else if (result_two == 0)
	{
		result_two = pthread_mutex_unlock(&philo->fork_right->mutex);
		pthread_mutex_unlock(&philo->fork_left->mutex);
		return (1);
	}
	return (0);
}
