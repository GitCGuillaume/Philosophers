/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchopin <gchopin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/23 11:24:11 by gchopin           #+#    #+#             */
/*   Updated: 2021/05/24 16:49:11 by gchopin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

void	*start_routine(void *args)
{
	t_philosopher *philosopher;
	int	result_one;
	int	result_two;

	philosopher = (t_philosopher *)args;
	result_one = pthread_mutex_lock(&philosopher->fork_left->mutex);
	if (result_one == 0)
	{
		printf("time %d has taken a fork\n", philosopher->number);
	}
	result_two = pthread_mutex_lock(&philosopher->fork_right->mutex);
	if (result_two == 0)
	{
		printf("time %d has taken a fork\n", philosopher->number);
	}
	result_one = pthread_mutex_unlock(&philosopher->fork_left->mutex);
	result_two = pthread_mutex_unlock(&philosopher->fork_right->mutex);
	//result_one = pthread_mutex_unlock(&parent->fork_left->mutex);
	//ft_putnbr_fd(result_two, 1);
	//usleep(100);
	return (NULL);
}

t_philosopher	**init_philosopher(int nb_philosopher, int argc, char **argv)
{
	t_philosopher	**philosopher;
	struct timeval	time;
	t_fork		**fork;
	int	i;

	i = 0;
	if (gettimeofday(&time, NULL) == -1)
		return (NULL);
	philosopher = malloc(sizeof(t_philosopher) * nb_philosopher);
	if (philosopher == NULL)
		return (NULL);
	fork = malloc(sizeof(t_fork) * nb_philosopher);
	if (fork == NULL)
		return(NULL);
	while (nb_philosopher > i)
	{
		philosopher[i] = malloc(sizeof(t_philosopher));
		if (philosopher[i] == NULL)
			return (NULL);
		fork[i] = malloc(sizeof(t_fork));
		if (fork[i]==NULL)
			return(NULL);
		if (pthread_mutex_init(&fork[i]->mutex, NULL) != 0)
			return (NULL);
		usleep(30);
		fork[i]->id = i + 1;
		philosopher[i]->number = i + 1;
		philosopher[i]->state.time_to_die = ft_atoi(argv[2]);
		philosopher[i]->state.time_to_eat = ft_atoi(argv[3]);
		philosopher[i]->state.time_to_sleep = ft_atoi(argv[4]);
		if (argc == 6)
			philosopher[i]->state.num_time_philo_must_eat = ft_atoi(argv[5]);
		else
		{
			philosopher[i]->state.nb_time = 0;
			philosopher[i]->state.num_time_philo_must_eat = 0;
		}
		philosopher[i]->alive = 1;
		philosopher[i]->eat = 0;
		philosopher[i]->think = 0;
		philosopher[i]->sleep = 0;
		philosopher[i]->nb_fork = 0;
		i++;
	}
	i = 0;
	while (nb_philosopher > i)
	{
		philosopher[i]->fork_right = fork[i];
		philosopher[i]->fork_left = NULL;
		if (nb_philosopher == 1)
		{
			fork[i + 1] = malloc(sizeof(t_fork));
			if (fork[i + 1]==NULL)
				return(NULL);
			philosopher[i]->fork_left = fork[i + 1];
		}
		else if (fork[i - 1] != NULL && i != 0)
		{
			philosopher[i]->fork_left = fork[i - 1];
		}
		else if (i == 0 && nb_philosopher > 1)
		{
			philosopher[i]->fork_left = fork[nb_philosopher - 1 - 1];
		}
		pthread_create(&philosopher[i]->thread, NULL, start_routine, philosopher[i]);
		//pthread_detach(philosopher[i]->thread);
		usleep(100);
		i++;
	}
	i = 0;
//	printf("timeval=%lu\nsizeof_philosopher=%lu\nphilosopher[i]=%lu\nfork=%lu\nfork[i]=%lu\n", sizeof(time), sizeof(philosopher), sizeof(philosopher[i]), sizeof(fork), sizeof(fork[i]));
	//printf("philosopher=%lu\n", sizeof(t_philosopher));
	return (NULL);
}

int	main(int argc, char **argv)
{
	int	nb_philosopher;

	if (check_args(argc, argv) == 0)
		return (0);
	nb_philosopher = ft_atoi(argv[1]);
//	printf("long int=%lu\nmutex=%lu\nint=%lu", sizeof(long int), sizeof(pthread_mutex_t), sizeof(int));
//	printf("alignement sizeof=%lu pthread=%lu\ntime=%lu fork=%lu\n", sizeof(t_philosopher), sizeof(pthread_t), sizeof(t_philosopher_time_state), sizeof(t_fork));
	init_philosopher(nb_philosopher, argc, argv);
	return (0);
}
