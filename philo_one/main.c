/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchopin <gchopin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/23 11:24:11 by gchopin           #+#    #+#             */
/*   Updated: 2021/06/25 20:47:47 by gchopin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

int	math_time(void)
{
	struct timeval time;
	int	res;
	int	value;

	res = gettimeofday(&time , NULL);
	if (res == 0)
		value = (time.tv_sec * 1000) + (time.tv_usec / 1000);
	else
		return (-1);
	return (value);
}

int	is_dying(long int current_time, long int time_simulation, long int time_to)
{
	if (current_time > (time_simulation + time_to))
	{
		return (1);
	}
	return (0);
}

int	take_fork_three(long int current_time, long int time_simulation, long int time_to, int philo_number)
{
	current_time = math_time();
	if (current_time != -1 && current_time > (time_simulation + time_to))
	{
		if (current_time != -1)
			printf("%ld %d died\n", current_time, philo_number);
		return (1);
	}
	else if (current_time != -1)
		printf("%ld %d has taken a fork\n", current_time, philo_number);
	return (0);
}

int	take_fork_two(t_philosopher *philo, int time)
{
	long int	current_time;

	current_time = math_time();
	time = take_fork_three(current_time, philo->state.time_simulation, philo->state.time_to_die, philo->number);
	if (time == 1)
	{
		pthread_mutex_destroy(&philo->fork_left->mutex);
		philo->dead = 1;
		return (1);
	}
	philo->nb_fork += 1;
	return (0);
}

int	take_fork(t_philosopher *philo)
{
	long int	current_time;
	int	time;
	int	result_one;
	int	result_two;

	result_one = pthread_mutex_lock(&philo->fork_left->mutex);
	if (result_one == 0)
	{
		result_two = take_fork_two(philo, time);
		if (result_two == 1)
			return (1);
	}
	else
		return (result_one);
	result_two = pthread_mutex_lock(&philo->fork_right->mutex);
	if (result_two == 0)
	{
		result_one = take_fork_two(philo, time);
		if (result_one == 1)
			return (1);
	}
	else
		return (result_two);
	return (0);
}

int	eating_two(t_philosopher *philo)
{
	long int current_time;
	current_time = math_time();
	if (current_time != -1)
	{
		printf("%ld %d died\n", current_time, philo->number);
		pthread_mutex_destroy(&philo->fork_left->mutex);
		pthread_mutex_destroy(&philo->fork_right->mutex);
		philo->dead = 1;
	}
	return (1);

}
int	eating(t_philosopher *philo)
{
	long int current_time;
	int	time_result;

	current_time = math_time();
	if (philo)
	{
		if (current_time != -1)
		{
			time_result = is_dying(current_time, philo->state.time_simulation, philo->state.time_to_die);
			if (time_result == 1)
			{
				return (eating_two(philo));
			}
			else
			{
				printf("%ld %d is eating\n", current_time, philo->number);
				usleep(philo->state.time_to_eat * 1000);
				philo->eat = 1;
				philo->state.time_simulation = math_time();
				pthread_mutex_unlock(&philo->fork_left->mutex);
				pthread_mutex_unlock(&philo->fork_right->mutex);
			}
		}
	}
	return (0);
}
void	*start_routine(void *args)
{
	t_philosopher *philosopher;
	int	result;

	result = 0;
	philosopher = (t_philosopher *)args;
	while (philosopher->nb_fork != 2 && result == 0 && philosopher->dead == 0)
		result = take_fork(philosopher);
	if (result == 0 && philosopher->nb_fork == 2 && philosopher->dead == 0)
		result = eating(philosopher);
	return (NULL);
}
void	free_all(t_philosopher **philo, t_fork **fork, int nb_philo)
{
	int	i;

	i = 0;
	while (nb_philo > i)
	{
		if (philo[i]->thread)
			pthread_detach(philo[i]->thread);
		i++;
	}
	i = 0;
	while (nb_philo > i)
	{
		if (philo[i] != NULL)
			free(philo[i]);
		if (fork[i] != NULL)
			free(fork[i]);
		i++;
	}
	if (fork)
		free(fork);
	if (philo)
		free(philo);
}

void	init_values_two(t_philosopher **philosopher, char **argv, int argc, int i)
{
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
	philosopher[i]->dead = 0;
	philosopher[i]->thread = 0;
}

int	init_values(t_philosopher **philosopher, t_fork **fork, char **argv, int argc)
{
	int	i;
	int	nb_philosopher;

	i = 0;
	nb_philosopher = ft_atoi(argv[1]);
	while (nb_philosopher > i)
	{
		philosopher[i] = malloc(sizeof(t_philosopher));
		if (philosopher[i] == NULL)
			return (0);
		fork[i] = malloc(sizeof(t_fork));
		if (fork[i] == NULL)
			return(0);
		if (pthread_mutex_init(&fork[i]->mutex, NULL) != 0)
			return (0);
		fork[i]->id = i + 1;
		philosopher[i]->number = i + 1;
		philosopher[i]->state.time_to_die = ft_atoi(argv[2]);
		philosopher[i]->state.time_to_eat = ft_atoi(argv[3]);
		philosopher[i]->state.time_to_sleep = ft_atoi(argv[4]);
		init_values_two(philosopher, argv, argc, i);
		i++;
	}
	return (1);
}



t_philosopher	**init_philosopher(int nb_philosopher, int argc, char **argv)
{
	t_philosopher	**philosopher;
	struct timeval	time;
	t_fork		**fork;
	int	i;
	int	result;
	int	time_result;

	i = 0;
	time_result = 1;
	philosopher = malloc(sizeof(t_philosopher) * nb_philosopher);
	if (philosopher == NULL)
		return (NULL);
	fork = malloc(sizeof(t_fork) * nb_philosopher);
	if (fork == NULL)
		return(NULL);
	result = init_values(philosopher, fork, argv, argc);
	if (result == 0)
		return (NULL);
	i = 0;
	while (nb_philosopher > i)
	{
		//time_result = gettimeofday(&philosopher[i]->state.time_simulation , NULL);
		time_result = math_time();
		if (time_result == -1)
		{
			free_all(philosopher, fork, nb_philosopher);
			return (NULL);
		}
		else
			philosopher[i]->state.time_simulation = time_result;
		philosopher[i]->fork_right = fork[i];
		philosopher[i]->fork_left = NULL;
		if (nb_philosopher == 1)
		{
			//fork[i + 1] = malloc(sizeof(t_fork));
			//if (fork[i + 1] == NULL)
			//	return(NULL);
			philosopher[i]->fork_left = fork[i + 1];
		}
		else if (i != 0 && fork[i - 1] != NULL)
		{
			philosopher[i]->fork_left = fork[i - 1];
		}
		else if (i == 0 && nb_philosopher > 1)
		{
			philosopher[i]->fork_left = fork[nb_philosopher - 1 - 1];
		}
		pthread_create(&philosopher[i]->thread, NULL, start_routine, philosopher[i]);
		usleep(100*1000);
		if (philosopher[i]->dead == 1)
		{
			free_all(philosopher, fork, nb_philosopher);
			return (NULL);
		}
		i++;
	}
	free_all(philosopher, fork, nb_philosopher);
	i = 0;
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
