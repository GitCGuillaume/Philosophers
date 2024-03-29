/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchopin <gchopin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/17 18:48:00 by gchopin           #+#    #+#             */
/*   Updated: 2021/12/02 10:47:55 by gchopin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

int	check_inputs_values(t_philosopher **philosopher, t_fork **fork,
	int argc, int nb_philosopher)
{
	if (philosopher[0]->state.time_to_die < 1
		|| philosopher[0]->state.time_to_eat < 1
		|| philosopher[0]->state.time_to_sleep < 1
		|| (philosopher[0]->state.nb_time_eat < 1 && argc == 6))
	{
		write(2, "Error\n", 6);
		free_all(philosopher, fork, nb_philosopher);
		return (0);
	}
	return (1);
}

int	check_nb_philosopher(int nb_philosopher)
{
	if (nb_philosopher < 1)
	{
		write(2, "Error\n", 6);
		return (0);
	}
	return (1);
}
