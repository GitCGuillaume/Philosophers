/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchopin <gchopin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/17 17:01:06 by gchopin           #+#    #+#             */
/*   Updated: 2021/12/13 11:09:37 by gchopin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher_bonus.h"

static void	clean_loop(t_philosopher **philo, int nb_philosopher, int *result)
{
	int	i;

	i = 0;
	if (philo)
	{
		while (nb_philosopher > i)
		{
			if (philo[i] != NULL)
			{
				if (philo[i]->eat_finish_exist == 1)
				{
					*result = sem_close(philo[i]->sem_eat_finish);
					philo[i]->eat_finish_exist = 2;
				}
				free(philo[i]);
			}
			i++;
		}
	}
}

static void	clean_sem_alone_two(t_philosopher *philo)
{
	if (philo)
	{
		if (philo->wait_loop_exist == 1)
		{
			sem_close(philo->wait_loop);
			philo->wait_loop_exist = 2;
		}
		if (philo->eat_wait_exist == 1)
		{
			sem_close(philo->sem_eat_wait);
			philo->eat_wait_exist = 2;
		}
	}
}

static void	clean_sem_alone(t_philosopher *philo)
{
	if (philo)
	{
		if (philo->fork_exist == 1)
		{
			sem_close(philo->fork);
			philo->fork_exist = 2;
		}
		if (philo->mutex_dead_exist == 1)
		{
			sem_close(philo->mutex_dead);
			philo->mutex_dead_exist = 2;
		}
		sem_close(philo->wait_loop);
		clean_sem_alone_two(philo);
	}
}

int	free_all(t_philosopher **philo, int nb_philosopher)
{
	char	*str;
	int		result;

	str = 0;
	result = -1;
	if (philo)
	{
		if (philo[0])
			clean_sem_alone(philo[0]);
		clean_loop(philo, nb_philosopher, &result);
		free(philo);
	}
	philo = NULL;
	if (result == -1)
		return (1);
	return (0);
}
