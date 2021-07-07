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

int	run_process(t_philosopher **philo, int nb_philosopher)
{
	pid_t	process;
	int	wait_pid;
	int	dead;
	int	i;
	int	wstatus;

	i = 0;
	wait_pid = 0;
	dead = 0;
	while (nb_philosopher > i)
	{
		philo[i]->process = fork();
		if (philo[i]->process == -1)
		{
			printf("Error no process returned\n");
			return (1);
		}
		else if (philo[i]->process == 0)
		{
			philo[i]->dead = &dead;
			philo[i]->state.start_time = math_time();
			//printf("Child process %d\n", philo[i]->process);
			start_routine(philo[i]);
	//		exit(0);
		}
		else
		{
		//	printf("Parent process %d\n", philo[i]->process);
		}
		i++;
		usleep(10);
	//	waitpid(philo[i]->process, &wstatus, 0);
	}
	/*if (dead == 1)
	{
		i = 0;
		while (nb_philosopher > i)
		{
			kill(philo[i]->process, SIGKILL);
			i++;
		}
	}*/
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

void	init_values(t_philosopher *philo)
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

int	main(int argc, char **argv)
{
	t_philosopher	**philosopher;
	sem_t	*sem_fork;
	pid_t	process;
	char	*str;
	char	*str_two;
	char	*str_itoa;
	int	i;
	int	nb_philosopher;

	str_two = 0;
	str_itoa = 0;
	
	str = 0;
	i = 0;
	nb_philosopher = ft_atoi(argv[1]);
	sem_fork = sem_open("name", O_CREAT, S_IRWXU, nb_philosopher);
	philosopher = malloc(sizeof(t_philosopher) * nb_philosopher);
	if (philosopher == NULL)
		return (1);
	while (nb_philosopher > i)
	{
		philosopher[i] = malloc(sizeof(t_philosopher));
		if (philosopher[i] == NULL)
			return (0);
		philosopher[i]->fork = sem_fork;
		str = ft_itoa(i);
		philosopher[i]->secure = sem_open(str, O_CREAT, S_IRWXU, 1);
		free(str);
		philosopher[i]->number = i + 1;
		init_values(philosopher[i]);
		init_values_time(philosopher[i], argc, argv);
		i++;
	}
	i = 0;
	while (nb_philosopher > i)
	{
		str_itoa = ft_itoa(i);
		if (str_itoa == NULL)
			return (1);
		str_two = ft_strjoin("b", str_itoa);
		if (str_two == NULL)
		{
			free(str_itoa);
			return (1);
		}
		philosopher[i]->mutex = sem_open(str_two, O_CREAT, S_IRWXU, 1);
		free(str_itoa);
		free(str_two);
		i++;
	}
	run_process(philosopher, nb_philosopher);
	i = 0;
	/*while (nb_philosopher > i)
	{
		str_itoa = ft_itoa(i);
		if (str_itoa == NULL)
			return (1);
		str_two = ft_strjoin("b", str_itoa);
		if (str_two == NULL)
		{
			free(str_itoa);
			return (1);
		}
		philosopher[i]->mutex = sem_open(str_two, O_CREAT, S_IRWXU, nb_philosopher);
		pthread_create(&philosopher[i]->thread, NULL,
			philo_dead_routine, philosopher[i]);
		free(str_itoa);
		free(str_two);
		i++;
		usleep(10);
	}
	i = 0;
	while (nb_philosopher > i)
	{
		pthread_join(philosopher[i]->thread, NULL);
		i++;
	}*/
	sem_close(sem_fork);
	sem_unlink("name");
	free_all(philosopher, nb_philosopher);
	return (0);
}
