/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   running_thread.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchopin <gchopin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/02 11:32:34 by gchopin           #+#    #+#             */
/*   Updated: 2021/12/02 14:00:53 by gchopin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

static int	prepare_thread(t_philosopher **philosopher, long int current_time, int *dead, int i)
{
	int	result;

	result = 1;
	philosopher[i]->state.start_time = current_time;
	philosopher[i]->state.time_simulation = current_time;
	philosopher[i]->state.current_time = current_time;
	if (current_time != -1)
	{
		result = pthread_create(&philosopher[i]->main_thread,
				NULL, start_routine, philosopher[i]);
	}
	if (result != 0)
	{
		*dead = 1;
		printf("Thread allocation failed.\n");
		return (1);
	}
	return (0);
}

void	end_running_thread(t_philosopher **philosopher,
	int argc, int *dead, int nb_philosopher)
{
	int	i;

	i = 0;
	while (nb_philosopher > i)
	{
		pthread_join(philosopher[i]->main_thread, NULL);
		i++;
	}
	if (argc == 6 && *dead == 0
		&& *philosopher[0]->everyone_eat == nb_philosopher)
	{
		printf("Everyone has eaten\n");
	}
}

int	running_thread(t_philosopher **philosopher, int argc, int nb_philosopher)
{
	long int		current_time;
	int			i;
	int			dead;
	int			nb_eat;
	int			result;

	i = 0;
	dead = 0;
	result = 1;
	nb_eat = 0;
	current_time = math_time();
	if (!philosopher || current_time == -1)
		return (0);
	while (nb_philosopher > i)
	{
		philosopher[i]->dead = &dead;
		philosopher[i]->everyone_eat = &nb_eat;
		philosopher[i]->nb_philosopher = nb_philosopher;
		result = prepare_thread(philosopher, current_time, &dead, i);
		if (result != 0)
			return (0);
		i++;
		usleep(10);
	}
	end_running_thread(philosopher, argc, &dead, nb_philosopher);
	return (1);
}
