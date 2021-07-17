/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchopin <gchopin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/17 16:50:20 by gchopin           #+#    #+#             */
/*   Updated: 2021/07/17 18:00:50 by gchopin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher_bonus.h"

int	init_sem(t_philosopher **philosopher, int nb_philosopher)
{
	sem_t	*mutex;
	sem_t	*sem_eat_wait;
	int		i;

	i = 0;
	mutex = sem_open("mutex1", O_CREAT, S_IRWXU, 1);
	if (mutex == SEM_FAILED)
		return (1);
	sem_unlink("mutex1");
	sem_eat_wait = sem_open("sem_eat_wait", O_CREAT, S_IRWXU, 0);
	if (sem_eat_wait == SEM_FAILED)
	{
		sem_close(mutex);
		return (1);
	}
	sem_unlink("sem_eat_wait");
	while (nb_philosopher > i)
	{
		philosopher[i]->mutex = mutex;
		philosopher[i]->mutex_exist = 1;
		philosopher[i]->sem_eat_wait = sem_eat_wait;
		philosopher[i]->eat_wait_exist = 1;
		i++;
	}
	return (0);
}

void	init_sem_exist(t_philosopher *philosopher,
	sem_t *sem_fork, sem_t *sem_dead)
{
	philosopher->fork_exist = 0;
	philosopher->mutex_exist = 0;
	philosopher->secure_exist = 0;
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
	philo->eat_finish_exist = 1;
	philo->dead = 0;
	philo->eat = 0;
	philo->sleep = 0;
	philo->nb_fork = 0;
	philo->think = 0;
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

int	init_values(t_philosopher **philosopher, int argc, char **argv, int i)
{
	char	*str;
	char	*str_two;

	str = ft_itoa(i);
	if (str == NULL)
		return (1);
	philosopher[i]->secure = sem_open(str, O_CREAT, S_IRWXU, 1);
	if (philosopher[i]->secure == SEM_FAILED)
		return (1);
	sem_unlink(str);
	philosopher[i]->secure_exist = 1;
	str_two = ft_strjoin(str, "b");
	free(str);
	if (str_two == NULL)
		return (1);
	philosopher[i]->sem_eat_finish = sem_open(str_two, O_CREAT, S_IRWXU, 0);
	if (philosopher[i]->sem_eat_finish == SEM_FAILED)
	{
		free(str_two);
		return (1);
	}
	sem_unlink(str_two);
	free(str_two);
	philosopher[i]->number = i + 1;
	return (0);
}

int	alloc_things(sem_t **sem_fork, sem_t **sem_dead,
	t_philosopher ***philosopher, char **argv)
{
	int	nb_philosopher;

	nb_philosopher = ft_atoi(argv[1]);
	if (nb_philosopher > 1)
		*sem_fork = sem_open("name", O_CREAT, S_IRWXU, nb_philosopher);
	else
		*sem_fork = sem_open("name", O_CREAT, S_IRWXU, 2);
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
