/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchopin <gchopin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/23 11:24:11 by gchopin           #+#    #+#             */
/*   Updated: 2021/06/29 20:47:59 by gchopin          ###   ########.fr       */
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
	/*current_time = math_time();
	if (current_time != -1 && current_time > (time_simulation + time_to))
	{
		//if (current_time != -1)
		//	printf("%ld %d died\n", current_time, philo_number);
		return (1);
	}
	else if (current_time != -1)
	*/	printf("%ld %d has taken a fork\n", current_time, philo_number);
	return (0);
}

int	take_fork_two(t_philosopher *philo, int time)
{
	long int	current_time;

	philo->state.current_time = math_time();
	/*time = take_fork_three(philo->state.current_time, philo->state.time_simulation, philo->state.time_to_die, philo->number);
	if (time == 1)
	{
		pthread_mutex_destroy(&philo->fork_left->mutex);
		philo->dead = 1;
		return (1);
	}*/
	printf("%ld %d has taken a fork\n", philo->state.current_time, philo->number);
	philo->nb_fork += 1;
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
		//pthread_detach(philo->thread);
	}
	return (1);

}

int	eating(t_philosopher *philo)
{
	long int current_time;
	int	time_result;

	philo->state.current_time = math_time();
	philo->eat = 1;
	if (philo && philo->eat == 1)
	{
		if (current_time != -1)
		{
			/*time_result = is_dying(current_time, philo->state.time_simulation, philo->state.time_to_die);
			if (time_result == 1)
			{
				return (eating_two(philo));
			}
			else
			{*/
				printf("%ld %d is eating\n", philo->state.current_time, philo->number);
				//philo->eat = 0;
				philo->state.time_simulation = math_time();
				usleep(philo->state.time_to_eat * 1000);
				philo->nb_fork = 0;
				pthread_mutex_unlock(&philo->fork_right->mutex);
				pthread_mutex_unlock(&philo->fork_left->mutex);
			//}
		}
	}
	return (0);
}

int	sleeping(t_philosopher *philo)
{
	long int	current_time;
	int	result;

	philo->state.current_time = math_time();
	philo->sleep = 1;
	if (philo)
	{
		if (current_time != -1)
		{
			/*result = is_dying(current_time, philo->state.time_simulation, philo->state.time_to_die);
			if (result == 1)
			{
				philo->dead = 1;
				printf("%ld %d died\n", current_time, philo->number);
				return (1);
			}
			else
			{*/

				pthread_mutex_lock(&philo->secure);
				printf("%ld %d is sleeping\n", philo->state.current_time, philo->number);
				usleep(philo->state.time_to_sleep * 1000);
				pthread_mutex_unlock(&philo->secure);
			//}
		}
	}
	return (0);
}

int	take_fork(t_philosopher *philo)
{
	long int	current_time;
	int	time;
	int	result_one;
	int	result_two;

	/*if (result_one == 0)
	{
		result_two = take_fork_two(philo, time);
		if (result_two == 1)
		{
			return (1);
		}
	}
	else
		return (result_one);
	*/
	result_one = pthread_mutex_lock(&philo->fork_left->mutex);
	philo->state.current_time = math_time();
	printf("%ld %d has taken a fork\n", philo->state.current_time, philo->number);
	philo->nb_fork += 1;
	result_two = pthread_mutex_lock(&philo->fork_right->mutex);
	philo->state.current_time = math_time();
	printf("%ld %d has taken a fork\n", philo->state.current_time, philo->number);
	philo->nb_fork += 1;
	
	/*if (result_two == 0)
	{
		result_one = take_fork_two(philo, time);
		if (result_one == 1)
		{
			return (1);
		}
	}
	else
		return (result_two);
	*/
	return (result_one);
}

/*
 ** Need to loop if philosopher is dead, for example if he can't get a fork and die
*/
/*
void	*routine_dead(void *args)
{
	t_philosopher **philosopher;
	t_main *main;
	int	result;
	int	i;
	long int	current_time;

	main = (t_main *)args;
	philosopher = main->philosopher;
	i = 0;
	while (main->dead == 0)
	{
		while (main->nb_philosopher > i)
		{
			pthread_mutex_lock(&main->secure);
			usleep(100);
			current_time = math_time();
			result = is_dying(current_time, philosopher[i]->state.time_simulation, philosopher[i]->state.time_to_die * 1000);
			if (result == 1)
			{
				printf("%ld %d died\n", current_time, philosopher[i]->number);
				main->dead = 1;
				pthread_mutex_unlock(&main->secure);
				return (NULL);
			}
			i++;
			result = pthread_mutex_unlock(&main->secure);
		}
		i = 0;
	}
	return (NULL);
}*/

void	*start_routine(void *args)
{
	t_philosopher *philosopher;
	int	result;

	result = 0;
	philosopher = (t_philosopher *)args;
	int i = 0;
	//while (philosopher->dead == 0)
	while (1)
	{
		if (philosopher->nb_fork == 0 && result == 0
				&& philosopher->dead == 0 && philosopher->eat == 0
				&& philosopher->sleep == 0)
			result = take_fork(philosopher);
		if (philosopher->nb_fork == 2 && result == 0
				&& philosopher->dead == 0 && philosopher->eat == 0
				&& philosopher->sleep == 0)
		{
			result = eating(philosopher);
		}
		if (philosopher->nb_fork == 0 && result == 0
				&& philosopher->dead == 0 && philosopher->eat == 1
				&& philosopher->sleep == 0)
			result = sleeping(philosopher);
		if (philosopher->nb_fork == 0 && result == 0
				&& philosopher->dead == 0 && philosopher->eat == 1
				&& philosopher->sleep == 1)
		{
			pthread_mutex_lock(&philosopher->secure);
			philosopher->eat = 0;
			philosopher->sleep = 0;
			printf("%d %d is thinking\n", math_time(), philosopher->number);
			pthread_mutex_unlock(&philosopher->secure);
		}
	}
	return (NULL);
}

void	free_all(t_philosopher **philo, t_fork **fork, int nb_philo)
{
	int	i;

	i = 0;
	while (nb_philo > i)
	{
		pthread_mutex_destroy(&philo[i]->secure);
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
		philosopher[i]->state.nb_time_eat = ft_atoi(argv[5]);
	else
	{
		philosopher[i]->state.nb_time = 0;
		philosopher[i]->state.nb_time_eat = 0;
	}
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
	t_main main;
	t_philosopher	**philosopher;
	struct timeval	time;
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
	while (nb_philosopher > i )
	{
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
			philosopher[i]->fork_left = fork[nb_philosopher - 1];
		}
		i++;
	}
	i = 0;
	while (nb_philosopher > i)
	{
		if (pthread_mutex_init(&philosopher[i]->secure, NULL) != 0)
			return (0);	
		i++;
	}
	i = 0;
	while (nb_philosopher > i)
	{
		philosopher[i]->state.time_simulation = math_time();
		if (philosopher[i]->state.time_simulation == -1)
		{
			free_all(philosopher, fork, nb_philosopher);
			return (NULL);
		}
		pthread_create(&philosopher[i]->thread, NULL, start_routine, philosopher[i]);
		pthread_detach(philosopher[i]->thread);
		usleep(100);
		//usleep(200);
		/*if (philosopher[i]->dead == 1)
		{
			dead = 1;
			free_all(philosopher, fork, nb_philosopher);
			return (NULL);
		}*/
		i++;
		
	}
	main.thread = 0;
	main.philosopher = philosopher;
	main.nb_philosopher = nb_philosopher;
	main.dead = 0;
	if (pthread_mutex_init(&main.secure, NULL) != 0)
		return (0);
	/*while (main.dead == 0)
	{
		pthread_create(&main.thread, NULL, routine_dead, &main);
		pthread_detach(main.thread);
	}*/
	
	i = 0;
	long int current_time;
	while (main.dead == 0)
	{
		while (main.nb_philosopher > i)
		{
			current_time = math_time();
			result = is_dying(current_time, philosopher[i]->state.time_simulation,
					philosopher[i]->state.time_to_die);
			if (result == 1)
			{
				printf("%ld %d died\n", current_time, philosopher[i]->number);
				main.dead = 1;
				return (NULL);
			}
			i++;
		}
		i = 0;
	}
	pthread_mutex_destroy(&main.secure);
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
