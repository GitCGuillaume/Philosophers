/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchopin <gchopin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/02 11:23:04 by gchopin           #+#    #+#             */
/*   Updated: 2021/07/02 13:47:05 by gchopin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

static void	free_malloc(t_philosopher **philo, t_fork **fork, int nb_philo)
{
	int	i;

	i = 0;
	while (nb_philo > i)
	{
		if (philo[i] != NULL)
			free(philo[i]);
		if (fork[i] != NULL)
			free(fork[i]);
		i++;
	}
	if (nb_philo == 1)
		free(fork[1]);
	if (fork)
		free(fork);
	if (philo)
		free(philo);
}

void	free_all(t_philosopher **philo, t_fork **fork,
		int nb_philo)
{
	int	i;

	i = 0;
	while (nb_philo > i)
	{
		if (philo[i]->secure_exist == 1)
		{
			pthread_mutex_unlock(&philo[i]->secure);
			pthread_mutex_destroy(&philo[i]->secure);
		}
		if (philo[i]->mutex_exist == 1)
		{
			pthread_mutex_unlock(&philo[i]->mutex);
			pthread_mutex_destroy(&philo[i]->mutex);
		}
		if (fork[i]->fork_exist == 1)
		{
			pthread_mutex_unlock(&fork[i]->mutex);
			pthread_mutex_destroy(&fork[i]->mutex);
		}		
		i++;
	}
	if (nb_philo == 1 && fork[1]->fork_exist == 1)
	{
		pthread_mutex_unlock(&fork[1]->mutex);
		pthread_mutex_destroy(&fork[1]->mutex);
	}
	free_malloc(philo, fork, nb_philo);
}
