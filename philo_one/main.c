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

void	*run_philosopher_two(t_philosopher **philosopher,
		t_fork **fork, int nb_philosopher, int argc)
{
	if (philosopher[0]->state.time_to_die < 1
		|| philosopher[0]->state.time_to_eat < 1
		|| philosopher[0]->state.time_to_sleep < 1
		|| (philosopher[0]->state.nb_time_eat < 1 && argc == 6))
	{
		printf("Values must be higher than 0\n");
		free_all(philosopher, fork, nb_philosopher, 0);
		return (NULL);
	}
	init_forks(philosopher, fork, nb_philosopher);
	mutex_init(philosopher, fork, nb_philosopher);
	running_thread(philosopher, nb_philosopher);
	if (argc == 6)
	{
		usleep(1);
		printf("Everyone has eaten\n");
	}
	free_all(philosopher, fork, nb_philosopher, 1);
	return (NULL);
}

t_philosopher	**run_philosopher(int nb_philosopher, int argc, char **argv)
{
	t_philosopher	**philosopher;
	t_fork			**fork;
	int				result;

	philosopher = NULL;
	fork = NULL;
	if (nb_philosopher < 1)
	{
		printf("Values must be higher than 0\n");
		return (NULL);
	}
	philosopher = malloc(sizeof(t_philosopher) * nb_philosopher);
	if (philosopher == NULL)
		return (NULL);
	fork = malloc(sizeof(t_fork) * nb_philosopher);
	if (fork == NULL)
		return (NULL);
	result = init_values(philosopher, fork, argv, argc);
	if (result == 0)
		return (NULL);
	run_philosopher_two(philosopher, fork, nb_philosopher, argc);
	return (NULL);
}

int	main(int argc, char **argv)
{
	int	nb_philosopher;

	if (check_args(argc, argv) == 0)
		return (0);
	nb_philosopher = ft_atoi(argv[1]);
	run_philosopher(nb_philosopher, argc, argv);
	return (0);
}
