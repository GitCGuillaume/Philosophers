/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchopin <gchopin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/23 11:24:11 by gchopin           #+#    #+#             */
/*   Updated: 2021/07/02 14:23:51 by gchopin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"
#include "philosopher_tools.h"

static int	run_philosopher_two(t_philosopher **philosopher,
		t_fork **fork, int nb_philosopher, int argc)
{
	int	result;

	if (!philosopher || !fork)
	{
		free_all(philosopher, fork, nb_philosopher);
		return (1);
	}
	result = check_inputs_values(philosopher, fork, argc, nb_philosopher);
	if (result == 0)
		return (1);
	result = init_forks(philosopher, fork, nb_philosopher);
	if (result == 0)
		return (1);
	result = init_mutex(philosopher, nb_philosopher);
	if (result == 0)
	{
		free_all(philosopher, fork, nb_philosopher);
		return (1);
	}
	result = running_thread(philosopher, argc, nb_philosopher);
	free_all(philosopher, fork, nb_philosopher);
	return (result);
}

static int	malloc_philo_fork(t_philosopher ***philosopher, t_fork ***fork, int nb_philosopher)
{
	*philosopher = malloc(sizeof(t_philosopher) * nb_philosopher);
	if (*philosopher == 0)
		return (0);
	if (nb_philosopher > 1)
		*fork = malloc(sizeof(t_fork) * nb_philosopher);
	else if (nb_philosopher == 1)
		*fork = malloc(sizeof(t_fork) * 2);
	if (*fork == NULL)
	{
		free(*philosopher);
		return (0);
	}
	return (1);
}

static int	run_philosopher(int nb_philosopher, int argc, char **argv)
{
	t_philosopher	**philosopher;
	t_fork			**fork;
	int				result;

	result = check_nb_philosopher(nb_philosopher);
	if (result == 0)
		return (0);
	result = malloc_philo_fork(&philosopher, &fork, nb_philosopher);
	if (result == 0)
		return (0);
	result = init_to_null(philosopher, fork, nb_philosopher);	
	if (result == 0)
	{
		free(fork);
		free(philosopher);
		return (0);
	}
	result = init_values(philosopher, fork, argv, argc);
	if (result == 0)
	{
		free_all(philosopher, fork, nb_philosopher);
		return (0);
	}
	result = run_philosopher_two(philosopher, fork, nb_philosopher, argc);
	return (result);
}

int	main(int argc, char **argv)
{
	int	nb_philosopher;
	int	result;

	if (check_args(argc, argv) == 0)
		return (0);
	nb_philosopher = ft_atoi(argv[1]);
	result = run_philosopher(nb_philosopher, argc, argv);
	if (result == 0)
		return (1);
	return (0);
}
