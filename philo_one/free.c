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
	if (fork)
		free(fork);
	if (philo)
		free(philo);
}

void	free_all(t_philosopher **philo, t_fork **fork,
		int nb_philo, int free_mutex)
{
	int	i;

	i = 0;
	while (nb_philo > i)
	{
		if (free_mutex == 1)
		{
			pthread_mutex_unlock(&philo[i]->secure);
			pthread_mutex_unlock(&philo[i]->mutex);
			pthread_mutex_destroy(&philo[i]->secure);
			pthread_mutex_destroy(&philo[i]->mutex);
		}
		pthread_mutex_unlock(&fork[i]->mutex);
		pthread_mutex_destroy(&fork[i]->mutex);
		i++;
	}
	free_malloc(philo, fork, nb_philo);
}
