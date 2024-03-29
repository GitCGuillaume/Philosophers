/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchopin <gchopin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/02 10:20:51 by gchopin           #+#    #+#             */
/*   Updated: 2021/12/14 14:22:01 by gchopin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"
#include "philosopher_tools.h"

int	init_mutex(t_philosopher **philosopher, int nb_philosopher)
{
	int				i;
	int				result;
	pthread_mutex_t	*display;

	i = 0;
	display = malloc(sizeof(pthread_mutex_t));
	if (!display)
		return (0);
	result = pthread_mutex_init(display, NULL);
	if (result != 0)
		return (0);
	while (nb_philosopher > i)
	{
		philosopher[i]->state.time_simulation = math_time();
		if (philosopher[i]->state.time_simulation == -1)
			return (0);
		philosopher[i]->display = display;
		philosopher[i]->display_exist = 1;
		i++;
	}
	return (1);
}

int	init_forks(t_philosopher **philosopher, t_fork **fork, int nb_philosopher)
{
	int	i;

	i = 0;
	while (nb_philosopher > i)
	{
		if (fork && fork[i] && fork[i]->fork_exist == 1)
			philosopher[i]->fork_right = fork[i];
		else
			philosopher[i]->fork_right = NULL;
		philosopher[i]->fork_left = NULL;
		if (i != 0 && fork[i - 1] != NULL && fork[i - 1]->fork_exist == 1)
			philosopher[i]->fork_left = fork[i - 1];
		else if (i == 0 && nb_philosopher > 1
			&& fork && fork[nb_philosopher - 1]
			&& fork[nb_philosopher - 1]->fork_exist == 1)
		{
			philosopher[i]->fork_left = fork[nb_philosopher - 1];
		}
		i++;
	}
	return (1);
}

void	init_values_two(t_philosopher **philosopher,
		char **argv, int argc, int i)
{
	philosopher[i]->number = i + 1;
	philosopher[i]->state.time_to_die = ft_atoi(argv[2]);
	philosopher[i]->state.time_to_eat = ft_atoi(argv[3]);
	philosopher[i]->state.time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
	{
		philosopher[i]->nb_time_active = 1;
		philosopher[i]->state.nb_time_eat = ft_atoi(argv[5]);
	}
	else
	{
		philosopher[i]->nb_time_active = 0;
		philosopher[i]->state.nb_time_eat = 0;
	}
	philosopher[i]->nb_time = 0;
	philosopher[i]->nb_time_reach = 0;
	philosopher[i]->eat = 0;
	philosopher[i]->think = 0;
	philosopher[i]->sleep = 0;
	philosopher[i]->nb_fork = 0;
	philosopher[i]->dead = 0;
	philosopher[i]->thread = 0;
	philosopher[i]->everyone_eat = 0;
}

int	init_values(t_philosopher **philosopher,
	t_fork **fork, char **argv, int argc)
{
	int	i;
	int	nb_philosopher;

	i = 0;
	nb_philosopher = ft_atoi(argv[1]);
	while (nb_philosopher > i)
	{
		if (pthread_mutex_init(&fork[i]->mutex, NULL) != 0)
			return (0);
		fork[i]->fork_exist = 1;
		fork[i]->id = i + 1;
		init_values_two(philosopher, argv, argc, i);
		i++;
	}
	return (1);
}
