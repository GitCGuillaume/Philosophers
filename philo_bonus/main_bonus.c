/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchopin <gchopin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/03 10:15:30 by gchopin           #+#    #+#             */
/*   Updated: 2021/12/14 14:22:37 by gchopin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher_bonus.h"

int	run_program(t_philosopher **philosopher, char **argv)
{
	int	result;

	result = 2;
	if (philosopher && argv)
	{
		result = run_process(philosopher, ft_atoi(argv[1]));
	}
	if (philosopher && argv && result == 2)
	{
		big_error(philosopher, argv);
		return (1);
	}
	if (philosopher && argv)
		free_all(philosopher, ft_atoi(argv[1]));
	return (result);
}

static int	loop_set_null(t_philosopher **philo, int nb_philosopher)
{
	int	i;

	i = 0;
	if (!philo)
		return (0);
	while (nb_philosopher > i)
	{
		philo[i] = NULL;
		i++;
	}
	return (1);
}

int	end_program(t_philosopher **philosopher, int argc, char **argv)
{
	int	result;

	if (!philosopher)
		return (1);
	result = check_inputs_values(philosopher, argc);
	if (result == 1)
	{
		free_all(philosopher, ft_atoi(argv[1]));
		return (1);
	}
	result = init_sem(philosopher, ft_atoi(argv[1]));
	if (result == 1)
	{
		free_all(philosopher, ft_atoi(argv[1]));
		return (1);
	}
	result = run_program(philosopher, argv);
	return (result);
}

t_philosopher	**start_program(int argc, char **argv,
	sem_t **sem_fork, sem_t **sem_dead)
{
	t_philosopher	**philosopher;
	int				i;

	if (!sem_fork || !sem_dead)
		return (NULL);
	i = 0;
	if (check_values(argv, argc) == 1)
		return (NULL);
	if (alloc_things(sem_fork, sem_dead, &philosopher, argv) == 1)
		return (NULL);
	loop_set_null(philosopher, ft_atoi(argv[1]));
	while (ft_atoi(argv[1]) > i)
	{
		philosopher[i] = malloc(sizeof(t_philosopher));
		if (philosopher[i] == NULL)
		{
			sem_close(*sem_fork);
			sem_close(*sem_dead);
			free_all(philosopher, ft_atoi(argv[1]));
			return (NULL);
		}
		init_sem_exist(philosopher[i], *sem_fork, *sem_dead);
		i++;
	}
	return (philosopher);
}

int	main(int argc, char **argv)
{
	t_philosopher	**philosopher;
	sem_t			*sem_fork;
	sem_t			*sem_dead;
	int				i;
	int				result;

	i = 0;
	if (!argv || check_args(argc, argv) == 1)
		return (1);
	philosopher = start_program(argc, argv, &sem_fork, &sem_dead);
	if (philosopher == NULL)
		return (1);
	while (ft_atoi(argv[1]) > i)
	{
		result = init_values(philosopher, i);
		init_values_two(philosopher[i], ft_atoi(argv[1]), argc, argv);
		if (result == 1)
		{
			free_all(philosopher, ft_atoi(argv[1]));
			return (1);
		}
		i++;
	}
	result = end_program(philosopher, argc, argv);
	return (result);
}
