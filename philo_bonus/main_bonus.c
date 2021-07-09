/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchopin <gchopin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/03 10:15:30 by gchopin           #+#    #+#             */
/*   Updated: 2021/07/07 14:50:54 by gchopin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher_bonus.h"

int	free_all(t_philosopher **philo, int nb_philo)
{
	char	*str;
	char	*str_two;
	int	i;

	i = 0;
	str = 0;
	str_two = 0;
	if (philo)
	{
		while (nb_philo > i)
		{
			if (philo[i] != NULL)
			{
				sem_close(philo[i]->secure);
				sem_close(philo[i]->mutex);
				str = ft_itoa(i);
				if (str == NULL)
					return (1);
				str_two = ft_strjoin("b", str);
				if (str_two == NULL)
				{
					free(str);
					return (1);
				}
				sem_unlink(str);
				sem_unlink(str_two);
				free(str);
				free(philo[i]);
			}
			i++;
		}
		free(philo);
	}
	philo = NULL;
	return (0);
}

int	run_process_two(t_philosopher *philo, long int current_time)
{
	if (philo->process == -1)
	{
		printf("Error no process returned\n");
		return (1);
	}
	else if (philo->process == 0)
	{
		philo->dead = 0;
		philo->state.start_time = current_time;
		philo->state.time_simulation = current_time;
		philo->state.current_time = current_time;
		pthread_create(&philo->thread, NULL,
			philo_dead_routine, philo);
		start_routine(philo);
		pthread_join(philo->thread, NULL);
		exit(0);
	}
	return (0);
}

int	run_process(t_philosopher **philo, int nb_philosopher)
{
	long int	current_time;
	int	i;
	int	wstatus;

	current_time = math_time();
	i = 0;
	while (nb_philosopher > i)
	{
		philo[i]->process = fork();
		run_process_two(philo[i], current_time);
		/*if (philo[i]->process == -1)
		{
			printf("Error no process returned\n");
			return (1);
		}
		else if (philo[i]->process == 0)
		{
			philo[i]->dead = 0;
			philo[i]->state.start_time = math_time();
			start_routine(philo[i]);
			exit(0);
		}*/
		i++;
		usleep(10);
	}
	waitpid(-1, &wstatus, 0);
	i = 0;
	while (nb_philosopher > i)
	{
		kill(philo[i]->process, SIGKILL);
		i++;
	}
	return (0);
}

void	init_values_time(t_philosopher *philo, int argc, char **argv)
{
	philo->state.current_time = math_time();
	philo->state.time_simulation = math_time();
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

void	init_values_two(t_philosopher *philo)
{
	philo->dead = 0;
	philo->eat = 0;
	philo->sleep = 0;
	philo->nb_fork = 0;
	philo->think = 0;
	philo->nb_time_active = 0;
	philo->nb_time = 0;
	philo->nb_time_reach = 0;
	philo->everyone = 0;
}

int	alloc_things(sem_t **sem_fork, sem_t **sem_test, t_philosopher ***philosopher, char **argv)
{
	int	nb_philosopher;

	nb_philosopher = ft_atoi(argv[1]);
	*sem_fork = sem_open("name", O_CREAT, S_IRWXU, nb_philosopher);
	*sem_test = sem_open("test", O_CREAT, S_IRWXU, 1);
	*philosopher = malloc(sizeof(t_philosopher) * nb_philosopher);
	if (philosopher == NULL)
		return (1);
	return (0);
}

int	init_values(t_philosopher **philosopher, int argc, char **argv, int i)
{
	char	*str;

	str = 0;
	philosopher[i] = malloc(sizeof(t_philosopher));
	if (philosopher[i] == NULL)
		return (1);
	str = ft_itoa(i);
	philosopher[i]->secure = sem_open(str, O_CREAT, S_IRWXU, 1);
	free(str);
	philosopher[i]->number = i + 1;
	init_values_two(philosopher[i]);
	init_values_time(philosopher[i], argc, argv);
	return (0);
}

int	init_mutex(t_philosopher **philosopher, int nb_philosopher)
{
	char	*str;
	char	*str_itoa;
	int	i;

	str = 0;
	str_itoa = 0;
	i = 0;
	while (nb_philosopher > i)
	{
		str_itoa = ft_itoa(i);
		if (str_itoa == NULL)
			return (1);
		str = ft_strjoin("b", str_itoa);
		if (str == NULL)
		{
			free(str_itoa);
			return (1);
		}
		philosopher[i]->mutex = sem_open(str, O_CREAT, S_IRWXU, 1);
		free(str_itoa);
		free(str);
		i++;
	}
	return (0);
}

int	run_program(t_philosopher **philosopher, char **argv, sem_t *sem_fork, sem_t *sem_test)
{
	int	result;

	result = run_process(philosopher, ft_atoi(argv[1]));
	if (result == 1)
		return (1);
	sem_close(sem_fork);
	sem_close(sem_test);
	sem_unlink("test");
	sem_unlink("name");
	result = free_all(philosopher, ft_atoi(argv[1]));
	if (result == 1)
		exit(1);	
	return (0);
}

int	main(int argc, char **argv)
{
	t_philosopher	**philosopher;
	sem_t	*sem_fork;
	sem_t	*sem_test;
	int	i;
	int	result;

	result = 0;
	i = 0;
	result = alloc_things(&sem_fork, &sem_test, &philosopher, argv);
	if (result == 1)
		return (1);	
	while (ft_atoi(argv[1]) > i)
	{
		result = init_values(philosopher, argc, argv, i);
		if (result == 1)
			return (1);
		philosopher[i]->fork = sem_fork;
		philosopher[i]->mutex_dead = sem_test;
		i++;
	}
	result = init_mutex(philosopher, ft_atoi(argv[1]));
	if (result == 1)
		return (1);
	result = run_program(philosopher, argv, sem_fork, sem_test);
	return (0);
}
