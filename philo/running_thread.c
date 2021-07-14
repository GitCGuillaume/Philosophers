/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   running_thread.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchopin <gchopin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/02 11:32:34 by gchopin           #+#    #+#             */
/*   Updated: 2021/07/07 11:02:44 by gchopin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

static int	prepare_thread(t_philosopher **philosopher, int *dead, int i)
{
	int	result;

	result = 1;
	philosopher[i]->state.start_time = math_time();
	if (philosopher[i]->state.start_time != -1)
		result = pthread_create(&philosopher[i]->main_thread,
			NULL, start_routine, philosopher[i]);
	if (result != 0)
	{
		*dead = 1;
		printf("Thread allocation failed.\n");
		return (1);
	}
	return (0);
}

void	end_running_thread(t_philosopher **philosopher, int argc, int *dead, int nb_philosopher)
{
	int	i;

	i = 0;
	while (nb_philosopher > i)
	{
		pthread_join(philosopher[i]->main_thread, NULL);
		i++;
	}
	if (argc == 6 && *dead == 0)
	{
		usleep(1);
		printf("Everyone has eaten\n");
	}
}
int	running_thread(t_philosopher **philosopher, int argc, int nb_philosopher)
{
	int			i;
	int			dead;
	int			result;

	i = 0;
	dead = 0;
	result = 1;
	if (!philosopher)
		return (1);
	while (nb_philosopher > i)
	{
		philosopher[i]->dead = &dead;
		result = prepare_thread(philosopher, &dead, i);
		if (result != 0)
			return (1);
		i++;
		usleep(15);
	}
	end_running_thread(philosopher, argc, &dead, nb_philosopher);
	return (0);
}
