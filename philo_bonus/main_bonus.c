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
	int	i;
	int	result;

	i = 0;
	str = 0;
	result = 0;	
	if (philo)
	{
		while (nb_philo > i)
		{
			if (philo[i] != NULL)
			{
				result = sem_close(philo[i]->secure);
				free(philo[i]);
			}
			i++;
		}
		free(philo);
	}
	philo = NULL;
	if (result == -1)
		return (1);
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
		sem_unlink("dead");
		sem_unlink("mutex1");
		sem_unlink("name");
		philo->dead = 0;
		philo->state.start_time = current_time;
		philo->state.time_simulation = current_time;
		philo->state.current_time = current_time;
		pthread_create(&philo->thread, NULL,
			philo_dead_routine, philo);
		pthread_detach(philo->thread);
		start_routine(philo);
		exit(3);
	}
	return (0);
}

int	run_process(t_philosopher **philo, int nb_philosopher)
{
	sem_t *wait_loop;
	long int	current_time;
	int	i;
	int	wstatus;
	int	result;

	wstatus = 0;
	current_time = math_time();
	i = 0;
	wait_loop = sem_open("test", O_CREAT, S_IRWXU, 0);
	sem_unlink("test");
	pthread_create(&philo->thread_eat, NULL,
		philo_eat_routine, philo);
	pthread_detach(philo->thread_eat);
	while (nb_philosopher > i)
	{
		philo[i]->wait_loop = wait_loop;
		philo[i]->process = fork();
		run_process_two(philo[i], current_time);
		i++;
		usleep(10);
	}
	sem_wait(wait_loop);
	sem_close(wait_loop);
	return (0);
}

void	init_values_time(t_philosopher *philo, int argc, char **argv)
{
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
	philo->eat_end = 0;
}

int	alloc_things(sem_t **sem_fork, sem_t **sem_dead, t_philosopher ***philosopher, char **argv)
{
	int	nb_philosopher;

	nb_philosopher = ft_atoi(argv[1]);
	*sem_fork = sem_open("name", O_CREAT, S_IRWXU, nb_philosopher);
	if (*sem_fork == SEM_FAILED)
		return (1);
	sem_unlink("name");
	*sem_dead = sem_open("dead", O_CREAT, S_IRWXU, 1);
	if (*sem_dead == SEM_FAILED)
		return (1);
	sem_unlink("dead");
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
	if (str == NULL)
		return (1);
	philosopher[i]->secure = sem_open(str, O_CREAT, S_IRWXU, 1);
	sem_unlink(str);
	free(str);
	if (philosopher[i]->secure == SEM_FAILED)
		return (1);
	philosopher[i]->number = i + 1;
	init_values_two(philosopher[i]);
	init_values_time(philosopher[i], argc, argv);
	return (0);
}

int	init_mutex(t_philosopher **philosopher, int nb_philosopher)
{
	sem_t	*mutex;
	sem_t	*eat_end;
	int	i;
	i = 0;
	mutex = sem_open("mutex1", O_CREAT, S_IRWXU, 1);
	while (nb_philosopher > i)
	{
		philosopher[i]->mutex = mutex;
		if (philosopher[i]->mutex == SEM_FAILED)
			return (1);
		i++;
	}
	i = 0;
	sem_unlink("mutex1");
	eat_end = sem_open("sem_eat_end", O_CREAT, S_IRWXU, nb_philosopher);
	while (nb_philosopher > i)
	{
		philosopher[i]->sem_eat_end = eat_end;
		if (philosopher[i]->mutex == SEM_FAILED)
			return (1);
		i++;
	}
	sem_unlink("sem_eat_end");
	return (0);
}

int	run_program(t_philosopher **philosopher, char **argv, sem_t *sem_fork, sem_t *sem_dead)
{
	int	result;
	int	i;

	i = 0;
	result = run_process(philosopher, ft_atoi(argv[1]));
	if (result == 1)
		return (1);
	while (ft_atoi(argv[1]) > i)
	{
		kill(philosopher[i]->process, SIGKILL);
		i++;
	}
	sem_close(sem_fork);
	sem_close(philosopher[0]->mutex);
	sem_close(sem_dead);
	result = free_all(philosopher, ft_atoi(argv[1]));
	if (result == 1)
		exit(1);	
	return (0);
}

int	main(int argc, char **argv)
{
	t_philosopher	**philosopher;
	sem_t	*sem_fork;
	sem_t	*sem_dead;
	int	i;
	int	result;

	result = 0;
	i = 0;
	result = alloc_things(&sem_fork, &sem_dead, &philosopher, argv);
	if (result == 1)
		return (1);	
	while (ft_atoi(argv[1]) > i)
	{
		result = init_values(philosopher, argc, argv, i);
		if (result == 1)
			return (1);
		philosopher[i]->fork = sem_fork;
		philosopher[i]->mutex_dead = sem_dead;
		i++;
	}
	result = init_mutex(philosopher, ft_atoi(argv[1]));
	if (result == 1)
		return (1);
	result = run_program(philosopher, argv, sem_fork, sem_dead);
	i = 0;
	return (0);
}
