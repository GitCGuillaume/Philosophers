/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines_secondary.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchopin <gchopin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/16 11:24:24 by gchopin           #+#    #+#             */
/*   Updated: 2021/12/07 15:36:48 by gchopin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

int	unlock_eating(t_philosopher *philo)
{
	int	result;

	result = 0;
	if (philo->fork_right && philo->fork_right->fork_exist == 1)
		result = pthread_mutex_unlock(&philo->fork_right->mutex);
	if (result != 0)
		printf("Error unlock mutex\n");
	if (philo->fork_left && philo->fork_left->fork_exist == 1)
		result = pthread_mutex_unlock(&philo->fork_left->mutex);
	if (result != 0)
		printf("Error unlock mutex\n");
	return (result);
}

int	take_fork_two(t_philosopher *philo)
{
	int	result_two;

	result_two = 0;
	//if (philo->fork_right && philo->fork_right->fork_exist && *philo->dead == 0)
	//{
		result_two = pthread_mutex_lock(&philo->fork_right->mutex);
		//pthread_mutex_lock(&philo->display);
		
		philo->nb_fork += 1;
	//}
	pthread_mutex_lock(&philo->display);
	if (philo && *philo->dead == 0) //&& result_two == 0
	//	&& philo->fork_right && philo->state.current_time != -1)
	{
		philo->state.current_time = math_time();
		printf("%ld %d has taken a fork\n",
			philo->state.current_time - philo->state.start_time, philo->number);
//	else if (result_two == 0 || *philo->dead == 1)
//	{
//		if (philo->fork_right)
//			pthread_mutex_unlock(&philo->fork_right->mutex);
//		if (philo->fork_left)
//			pthread_mutex_unlock(&philo->fork_left->mutex);
//		return (1);
	}
	pthread_mutex_unlock(&philo->display);
	return (0);
}
