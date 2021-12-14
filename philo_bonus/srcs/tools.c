/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchopin <gchopin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/16 17:53:25 by gchopin           #+#    #+#             */
/*   Updated: 2021/12/14 13:48:35 by gchopin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher_bonus.h"

int	check_inputs_values(t_philosopher **philosopher,
	int argc)
{
	if (!philosopher || !philosopher[0])
		return (1);
	if (philosopher[0]->state.time_to_die < 1
		|| philosopher[0]->state.time_to_eat < 1
		|| philosopher[0]->state.time_to_sleep < 1
		|| (philosopher[0]->state.nb_time_eat < 1 && argc == 6))
	{
		printf("Values must be higher than 0\n");
		return (1);
	}
	return (0);
}

int	check_values(char **argv, int argc)
{
	if (ft_atoi(argv[1]) > 2147483647 || ft_atoi(argv[2]) > 2147483647
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
	if (ft_atoi(argv[1]) < 1)
	{
		printf("Values must be higher than 0\n");
		return (1);
	}
	return (0);
}

void	big_error(t_philosopher **philosopher, char **argv)
{
	int	i;

	i = 0;
	if (philosopher && argv)
	{
		if (philosopher[0]->thread_eat)
		{
			i = 0;
			while (ft_atoi(argv[1]) > i++)
			{
				sem_post(philosopher[0]->sem_eat_wait);
				usleep(10);
			}
			pthread_join(philosopher[0]->thread_eat, NULL);
		}
		printf("Error\n");
		free_all(philosopher, ft_atoi(argv[1]));
	}
}
