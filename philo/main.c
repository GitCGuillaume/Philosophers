/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchopin <gchopin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/23 11:24:11 by gchopin           #+#    #+#             */
/*   Updated: 2021/12/14 09:26:03 by gchopin          ###   ########.fr       */
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
		return (0);
	}
	result = check_inputs_values(philosopher, fork, argc, nb_philosopher);
	if (result == 0)
		return (0);
	result = init_forks(philosopher, fork, nb_philosopher);
	if (result == 0)
		return (0);
	result = init_mutex(philosopher, nb_philosopher);
	if (result == 0)
	{
		free_all(philosopher, fork, nb_philosopher);
		return (0);
	}
	result = running_thread(philosopher, argc, nb_philosopher);
	free_all(philosopher, fork, nb_philosopher);
	return (result);
}

static int	malloc_philo_fork(t_philosopher ***philosopher,
	t_fork ***fork, int nb_philosopher)
{
	*philosopher = malloc(sizeof(t_philosopher) * nb_philosopher);
	if (!fork)
		return (0);
	if (!philosopher || *philosopher == 0)
		return (0);
	if (nb_philosopher > 1)
		*fork = malloc(sizeof(t_fork) * nb_philosopher);
	else if (nb_philosopher == 1)
		*fork = malloc(sizeof(t_fork));
	if (fork && *fork == NULL)
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
	ssize_t	nb_philosopher;
	int		result;

	if (check_args(argc, argv) == 0)
		return (1);
	nb_philosopher = ft_atoi(argv[1]);
	if (nb_philosopher > 2147483647 || ft_atoi(argv[2]) > 2147483647
		|| ft_atoi(argv[3]) > 2147483647 || ft_atoi(argv[4]) > 2147483647)
	{
		printf("Error\n");
		return (1);
	}
	if (argc == 6)
	{
		if (ft_atoi(argv[5]) > 2147483647)
		{
			printf("Error\n");
			return (1);
		}
	}
	result = run_philosopher((int)nb_philosopher, argc, argv);
	if (result == 0)
		return (1);
	return (0);
}
