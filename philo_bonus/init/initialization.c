/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchopin <gchopin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/17 16:50:20 by gchopin           #+#    #+#             */
/*   Updated: 2021/12/13 11:10:01 by gchopin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher_bonus.h"

int	init_sem(t_philosopher **philosopher, int nb_philosopher)
{
	sem_t	*sem_eat_wait;
	sem_t	*sem_eat_finish;
	int		i;

	i = 0;
	sem_eat_wait = sem_open("sem_eat_wait", O_CREAT, S_IRWXU, 0);
	if (sem_eat_wait == SEM_FAILED)
		return (1);
	sem_unlink("sem_eat_wait");
	sem_eat_finish = sem_open("sem_eat_finish", O_CREAT, S_IRWXU, 0);
	if (sem_eat_finish == SEM_FAILED)
	{
		sem_close(sem_eat_wait);
		return (1);
	}
	sem_unlink("sem_eat_finish");
	while (nb_philosopher > i)
	{
		philosopher[i]->sem_eat_finish = sem_eat_finish;
		philosopher[i]->eat_finish_exist = 1;
		philosopher[i]->sem_eat_wait = sem_eat_wait;
		philosopher[i]->eat_wait_exist = 1;
		i++;
	}
	return (0);
}

void	init_sem_exist(t_philosopher *philosopher,
	sem_t *sem_fork, sem_t *sem_dead)
{
	philosopher->wait_loop = 0;
	philosopher->fork_exist = 0;
	philosopher->mutex_dead_exist = 0;
	philosopher->wait_loop_exist = 0;
	philosopher->eat_wait_exist = 0;
	philosopher->eat_finish_exist = 0;
	philosopher->fork = sem_fork;
	philosopher->fork_exist = 1;
	philosopher->mutex_dead = sem_dead;
	philosopher->mutex_dead_exist = 1;
}

void	init_values_two(t_philosopher *philo,
	int nb_philosopher, int argc, char **argv)
{
	if (philo)
	{
		philo->thread_eat = 0;
		philo->nb_time_active = 0;
		philo->nb_time = 0;
		philo->nb_time_reach = 0;
		philo->nb_philosopher = nb_philosopher;
		philo->state.time_to_die = ft_atoi(argv[2]);
		philo->state.time_to_eat = ft_atoi(argv[3]);
		philo->state.time_to_sleep = ft_atoi(argv[4]);
		if (argc == 6)
		{
			philo->nb_time_active = 1;
			philo->state.nb_time_eat = ft_atoi(argv[5]);
		}
		else
		{
			philo->nb_time_active = 0;
			philo->state.nb_time_eat = 0;
		}
	}
}

int	init_values(t_philosopher **philosopher, int i)
{
	if (philosopher[i])
	{
		philosopher[i]->finish = 0;
		philosopher[i]->number = i + 1;
		philosopher[i]->eat_finish_exist = 1;
		philosopher[i]->dead = 0;
		philosopher[i]->eat = 0;
		philosopher[i]->sleep = 0;
		philosopher[i]->nb_fork = 0;
		philosopher[i]->think = 0;
	}
	return (0);
}

int	alloc_things(sem_t **sem_fork, sem_t **sem_dead,
	t_philosopher ***philosopher, char **argv)
{
	int	nb_philosopher;

	if (!philosopher || !sem_fork || !sem_dead || !argv)
		return (1);
	nb_philosopher = ft_atoi(argv[1]);
	if (nb_philosopher > 0)
		*sem_fork = sem_open("name", O_CREAT, S_IRWXU, nb_philosopher);
	if (*sem_fork == SEM_FAILED)
		return (1);
	sem_unlink("name");
	*sem_dead = sem_open("dead", O_CREAT, S_IRWXU, 1);
	if (*sem_dead == SEM_FAILED)
	{
		sem_close(*sem_fork);
		return (1);
	}
	sem_unlink("dead");
	*philosopher = malloc(sizeof(t_philosopher) * nb_philosopher);
	if (philosopher == NULL)
	{
		sem_close(*sem_fork);
		sem_close(*sem_dead);
		return (1);
	}
	return (0);
}
