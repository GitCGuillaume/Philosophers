/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchopin <gchopin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/23 11:24:11 by gchopin           #+#    #+#             */
/*   Updated: 2021/07/01 21:22:37 by gchopin          ###   ########.fr       */
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

int	eating(t_philosopher *philo)
{
	philo->state.current_time = math_time();
	philo->state.time_simulation = math_time();
	if (*philo->dead == 0)
		printf("%ld %d is eating\n", philo->state.current_time - philo->state.start_time, philo->number);
	usleep(philo->state.time_to_eat * 1000);
	if (philo->nb_time_active == 1)
	{
		philo->nb_time = philo->nb_time + 1;
		if (philo->nb_time >= philo->state.nb_time_eat && philo->nb_time_reach == 0)
			philo->nb_time_reach = philo->nb_time_reach + 1;
	}
	philo->nb_fork = 0;
	philo->eat = 1;
	pthread_mutex_unlock(&philo->fork_right->mutex);
	pthread_mutex_unlock(&philo->fork_left->mutex);
	return (0);
}

int	sleeping(t_philosopher *philo)
{
	pthread_mutex_lock(&philo->secure);
	philo->state.current_time = math_time();
	philo->sleep = 1;
	if (*philo->dead == 0)
		printf("%ld %d is sleeping\n", philo->state.current_time - philo->state.start_time, philo->number);
	usleep(philo->state.time_to_sleep * 1000);
	pthread_mutex_unlock(&philo->secure);
	return (0);
}

int	take_fork(t_philosopher *philo)
{
	int	result_one;
	int	result_two;

	result_one = pthread_mutex_lock(&philo->fork_left->mutex);
	philo->state.current_time = math_time();
	if (philo->dead == 0)
		printf("%ld %d has taken a fork\n", philo->state.current_time - philo->state.start_time, philo->number);
	philo->nb_fork += 1;
	
	result_two = pthread_mutex_lock(&philo->fork_right->mutex);
	philo->state.current_time = math_time();
	if (*philo->dead == 0)
		printf("%ld %d has taken a fork\n", philo->state.current_time - philo->state.start_time, philo->number);
	philo->nb_fork += 1;
	return (result_one);
}
/*
void	*death_routine(void *args)
{
	t_philosopher **philosopher;
	t_main *main;
	long int	current_time;
	int	result;
	int	i;

	main = (t_main *)args;
	philosopher = main->philosopher;
	i = 0;
	//while (main->dead == 0 && main->eat_at_least == 0)
	//{
		pthread_mutex_lock(&main->anti_overload);
		main->eat_at_least = 1;
		while (main->nb_philosopher > i && main->dead == 0)
		{
			current_time = math_time();
			if (main->eat_at_least == 1 && main->argc == 6
					&& philosopher[i]->nb_time < philosopher[i]->state.nb_time_eat)
				main->eat_at_least = 0;
			result = is_dying(current_time, philosopher[i]->state.time_simulation,
					philosopher[i]->state.time_to_die);
			if (result == 1)
			{
				main->dead = 1;
				printf("%ld %d died\n", current_time - philosopher[i]->state.start_time, philosopher[i]->number);
				pthread_detach(philosopher[i]->thread);
				pthread_mutex_unlock(&main->anti_overload);
				pthread_detach(main->thread);
				return (NULL);
			}
			if (main->eat_at_least == 1 && main->argc == 6)
			{
				printf("%ld %d everyone has eaten\n", current_time, philosopher[i]->number);
				pthread_detach(philosopher[i]->thread);
				pthread_mutex_unlock(&main->anti_overload);
				pthread_detach(main->thread);
				return (NULL);
			}
			else
				main->eat_at_least = 0;
			i++;
		}
		i = 0;
		pthread_mutex_unlock(&main->anti_overload);
	//}
	return (NULL);
}*/
/*
void	is_dead(t_philosopher *philosopher)
{
	long	int	current_time;
	int	result;

	pthread_mutex_lock(&philosopher->secure);
	current_time = math_time();
	result = is_dying(current_time, philosopher->state.time_simulation,
		philosopher->state.time_to_die);
	if (result == 1)
	{
		*philosopher->dead = 1;
		printf("%ld %d died\n", current_time - philosopher->state.start_time, philosopher->number);
		pthread_detach(philosopher->thread);
		pthread_mutex_unlock(&philosopher->secure);
	}
	pthread_mutex_unlock(&philosopher->secure);
}*/

void	*philo_dead_routine(void *args)
{
	t_philosopher	*philo;
	long int	current_time;
	int	result;

	philo = (t_philosopher *)args;
	result = 0;
	while (result == 0 && *philo->dead == 0)
	{
		pthread_mutex_lock(&philo->mutex);
		if (philo->nb_time_reach > 0)
		{
			//current_time = math_time();
			//printf("%ld everyone has eaten\n", current_time - philo->state.start_time);
			//*philo->dead = 1;
			pthread_mutex_unlock(&philo->mutex);
			pthread_mutex_unlock(&philo->mutex);
			return (NULL);
		}
		current_time = math_time();
		result = is_dying(current_time, philo->state.time_simulation,
			philo->state.time_to_die);
		if (result == 1)
		{
			current_time = math_time();
			*philo->dead = 1;
			printf("%ld %d died\n", current_time - philo->state.start_time, philo->number);
			pthread_mutex_unlock(&philo->mutex);
			return (NULL);
		}
		pthread_mutex_unlock(&philo->mutex);
		usleep(5);
	}
	return (NULL);
}

void	*start_routine(void *args)
{
	t_philosopher *philosopher;
	//t_alive	alive;
	int	result;
	int	current_time;

	result = 0;
	philosopher = (t_philosopher *)args;
	while (*philosopher->dead == 0)
	{
		if (philosopher->nb_fork == 0 && result == 0
				&& *philosopher->dead == 0 && philosopher->eat == 0
				&& philosopher->sleep == 0)
		{
			result = take_fork(philosopher);
		}
		if (philosopher->nb_fork == 2 && result == 0
				&& *philosopher->dead == 0 && philosopher->eat == 0
				&& philosopher->sleep == 0)
		{
			result = eating(philosopher);
		}
		if (philosopher->nb_fork == 0 && result == 0
				&& *philosopher->dead == 0 && philosopher->eat == 1
				&& philosopher->sleep == 0)
		{
			result = sleeping(philosopher);
		}
		if (philosopher->nb_fork == 0 && result == 0
				&& *philosopher->dead == 0 && philosopher->eat == 1
				&& philosopher->sleep == 1)
		{
			pthread_mutex_lock(&philosopher->secure);
			current_time = math_time();
			philosopher->eat = 0;
			philosopher->sleep = 0;
			if (*philosopher->dead == 0)
				printf("%ld %d is thinking\n", current_time - philosopher->state.start_time, philosopher->number);
			pthread_mutex_unlock(&philosopher->secure);
		}
	}
	return (NULL);
}

void	free_all(t_philosopher **philo, t_fork **fork, int nb_philo, int free_mutex)
{
	int	i;

	i = 0;
		while (nb_philo > i)
		{
			if (free_mutex == 1)
			{
				pthread_mutex_unlock(&philo[i]->secure);
				pthread_mutex_unlock(&philo[i]->mutex);
				pthread_mutex_destroy(&philo[i]->secure);
				pthread_mutex_destroy(&philo[i]->mutex);
			}
			pthread_mutex_unlock(&fork[i]->mutex);
			pthread_mutex_destroy(&fork[i]->mutex);
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
	philosopher[i]->everyone = 0;
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
	t_main main;
	t_philosopher	**philosopher;
	t_fork		**fork;
	int	i;
	int	result;
	int	time_result;
	int	dead;
	int	eat_finish;
	
	i = 0;
	time_result = 1;
	dead = 0;
	eat_finish = 0;
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
		return(NULL);
	result = init_values(philosopher, fork, argv, argc);
	if (result == 0)
		return (NULL);
	if (philosopher[0]->state.time_to_die < 1 || philosopher[0]->state.time_to_eat < 1
			|| philosopher[0]->state.time_to_sleep < 1 || (philosopher[0]->state.nb_time_eat < 1 && argc == 6))
	{
		printf("Values must be higher than 0\n");
		free_all(philosopher, fork, nb_philosopher, 0);
		return (NULL);
	}
	i = 0;
	while (nb_philosopher > i )
	{
		philosopher[i]->fork_right = fork[i];
		philosopher[i]->fork_left = NULL;
		if (nb_philosopher == 1)
		{
			fork[i + 1] = malloc(sizeof(t_fork));
			if (fork[i + 1] == NULL)
				return (NULL);
			philosopher[i]->fork_left = fork[i + 1];
		}
		else if (i != 0 && fork[i - 1] != NULL)
		{
			philosopher[i]->fork_left = fork[i - 1];
		}
		else if (i == 0 && nb_philosopher > 1)
		{
			philosopher[i]->fork_left = fork[nb_philosopher - 1];
		}
		i++;
	}
	i = 0;
	while (nb_philosopher > i)
	{
		if (pthread_mutex_init(&philosopher[i]->secure, NULL) != 0)
			return (0);	
		if (pthread_mutex_init(&philosopher[i]->mutex, NULL) != 0)
			return (0);	
		philosopher[i]->state.time_simulation = math_time();
		if (philosopher[i]->state.time_simulation == -1)
		{
			free_all(philosopher, fork, nb_philosopher, 1);
			return (NULL);
		}
		i++;
	}
	i = 0;
	main.dead = 0;
	main.thread = 0;
	main.eat_at_least = 0;
	dead = 0;
	while (nb_philosopher > i)
	{
		philosopher[i]->dead = &dead;
		philosopher[i]->state.start_time = math_time();
		pthread_create(&main.thread, NULL, start_routine, philosopher[i]);
		pthread_detach(main.thread);
		pthread_create(&philosopher[i]->thread, NULL, philo_dead_routine, philosopher[i]);
		i++;
		usleep(10);
	}
	i = 0;
	while (nb_philosopher > i)
	{
		pthread_join(philosopher[i]->thread, NULL);
		i++;
	}
	if (argc == 6)
	{
		usleep(1);
		printf("Everyone has eaten\n");
	}
	/*if (argc == 6)
	{
		i = 0;
		int	everyone = -1;
		while (main.eat_at_least == 0)
		{
			while (nb_philosopher > i)
			{
				i++;
				if (philosopher[i]->nb_time_reach == 1 && (everyone == -1 || everyone == 1))
					everyone = 1;
				else
					everyone = 0;
				usleep(philosopher[0]->state.time_to_eat >> 2);
			}
			if (everyone == 1)
				philosopher[i]->everyone = 1;
			i = 0;
		}
	}*/
	/*while (main.dead == 0)
	{
	//	pthread_create(&main.thread, NULL, death_routine, &main);
	//	pthread_detach(main.thread);
		usleep(1);
	}*/
	//pthread_mutex_destroy(&main.anti_overload);
	i = 0;
	//int	eat_at_least;
	/*
	long int current_time;

	while (main.dead == 0 && eat_at_least == 0)
	{
		eat_at_least = 1;
		while (main.nb_philosopher > i && main.dead == 0)
		{
			current_time = math_time();
			if (eat_at_least == 1 && argc == 6
					&& philosopher[i]->nb_time < philosopher[i]->state.nb_time_eat)
				eat_at_least = 0;
			result = is_dying(current_time, philosopher[i]->state.time_simulation,
					philosopher[i]->state.time_to_die);
			if (result == 1)
			{
				main.dead = 1;
				pthread_detach(philosopher[i]->thread);
				printf("%ld %d died\n", current_time, philosopher[i]->number);
				break;
			}
			if (eat_at_least == 1 && argc == 6)
			{
				pthread_detach(philosopher[i]->thread);
				printf("%ld %d everyone has eaten\n", current_time, philosopher[i]->number);
				break;
			}
			else
				eat_at_least = 0;
			i++;
		}
		i = 0;
	}*/
	free_all(philosopher, fork, nb_philosopher, 1);
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
