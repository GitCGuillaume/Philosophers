/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization_two.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchopin <gchopin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/17 18:45:54 by gchopin           #+#    #+#             */
/*   Updated: 2021/07/17 18:45:56 by gchopin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"
/*
 ** Need to put everyone to NULL, easier to free
*/

static int	loop_set_null(t_philosopher **philo, t_fork **fork,
	int nb_philosopher)
{
	int	i;

	i = 0;
	if (!philo || !fork)
		return (0);
	while (nb_philosopher > i)
	{
		philo[i] = NULL;
		fork[i] = NULL;
		i++;
	}
	if (nb_philosopher == 1)
		fork[1] = NULL;
	return (1);
}

/*
 ** Give a fork to the only philosopher
*/

static int	give_fork_philo(t_philosopher **philo, t_fork **fork,
	int nb_philosopher)
{
	if (!philo || !fork)
		return (0);
	if (nb_philosopher == 1)
	{
		fork[1] = malloc(sizeof(t_philosopher));
		if (fork[1] == NULL)
		{
			free_init_null(philo, fork, nb_philosopher);
			return (0);
		}
		fork[1]->fork_exist = 0;
	}
	return (1);
}

static int	set_values(t_philosopher **philo, t_fork **fork,
	int nb_philosopher, int i)
{
	if (!philo || !fork)
		return (0);
	philo[i] = malloc(sizeof(t_philosopher));
	if (philo[i] == NULL)
	{
		free_init_null(philo, fork, nb_philosopher);
		return (0);
	}
	philo[i]->fork_left = NULL;
	philo[i]->fork_right = NULL;
	//philo[i]->secure_exist = 0;
	//philo[i]->mutex_exist = 0;
	//philo[i]->mutex_eat_exist = 0;
	fork[i] = malloc(sizeof(t_fork));
	if (fork[i] == NULL)
	{
		free_init_null(philo, fork, nb_philosopher);
		return (0);
	}		
	fork[i]->fork_exist = 0;
	return (1);
}

int	init_to_null(t_philosopher **philo, t_fork **fork, int nb_philosopher)
{
	int	i;
	int	result;

	i = 0;
	result = 0;
	if (!philo || !fork)
		return (0);
	result = loop_set_null(philo, fork, nb_philosopher);
	if (result == 0)
		return (0);
	while (nb_philosopher > i)
	{
		result = set_values(philo, fork, nb_philosopher, i);
		if (result == 0)
			return (0);
		i++;
	}
	result = give_fork_philo(philo, fork, nb_philosopher);
	return (result);
}
