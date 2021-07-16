/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchopin <gchopin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/03 10:15:30 by gchopin           #+#    #+#             */
/*   Updated: 2021/07/16 21:32:17 by gchopin          ###   ########.fr       */
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
        if (philo[0])
        {
            if (philo[0]->fork_exist == 1)
            {
                sem_close(philo[0]->fork);
                philo[0]->fork_exist = 2;
            }
            if (philo[0]->mutex_exist == 1)
            {
                sem_close(philo[0]->mutex);
                philo[0]->mutex_exist = 2;
            }
            if (philo[0]->mutex_dead_exist == 1)
            {
                sem_close(philo[0]->mutex_dead);
                philo[0]->mutex_dead_exist = 2;
            }
            if (philo[0]->wait_loop_exist == 1)
            {
                sem_close(philo[0]->wait_loop);
                philo[0]->wait_loop_exist = 2;
            }
            if (philo[0]->eat_wait_exist == 1)
            {
                sem_close(philo[0]->sem_eat_wait);
                philo[0]->eat_wait_exist = 2;
            }
        }        
        while (nb_philo > i)
        {
            if (philo[i] != NULL)
            {
                if (philo[i]->secure_exist == 1)
                {
                    result = sem_close(philo[i]->secure);
                    philo[i]->secure_exist = 2;
                }
                if (philo[i]->eat_finish_exist == 1)
                {
                    result = sem_close(philo[i]->sem_eat_finish);
                    philo[i]->eat_finish_exist = 2;
                }
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
	pthread_t	thread_eat;
	long int	current_time;
	int	i;
	int	wstatus;
	int	result;

	i = 0;
	wstatus = 0;
	current_time = math_time();
	if (current_time == -1)
		return (2);
	wait_loop = sem_open("wait_loop", O_CREAT, S_IRWXU, 0);
	if (wait_loop == SEM_FAILED)
		return (2);
	sem_unlink("wait_loop");
	philo[i]->wait_loop_exist = 1;
	if (philo[0]->nb_time_active == 1)
	{
		result = pthread_create(&thread_eat, NULL, philo_eat_routine, philo[0]);
		if (result != 0)
		{
			philo[0]->wait_loop = wait_loop;
			return (2);
		}
		pthread_detach(thread_eat);
	}
	while (nb_philosopher > i)
	{
		philo[i]->wait_loop = wait_loop;
		philo[i]->process = fork();
		result = run_process_two(philo[i], current_time);
		if (result == 1)
		{
			i = 0;
			while (philo[0]->nb_time_active == 1 && nb_philosopher > i)
			{
				sem_post(philo[0]->sem_eat_wait);
				sem_post(philo[0]->sem_eat_finish);
				i++;
			}
			sem_post(wait_loop);
			usleep(800);
			return (2);
		}
		i++;
		usleep(10);
	}
	sem_wait(wait_loop);
	if (philo[0]->nb_time_active == 1)
	{
		i = 0;
		while (nb_philosopher > i)
		{
			sem_post(philo[0]->sem_eat_wait);
			i++;
			usleep(10);
		}
		//anti valgrind  pthreadleak
		usleep(800);
	}
	
	return (0);
}

void	init_values_two(t_philosopher *philo, int nb_philosopher, int argc, char **argv)
{
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

void	init_sem_exist(t_philosopher *philosopher)
{
	philosopher->fork_exist = 0;
	philosopher->mutex_exist = 0;
	philosopher->secure_exist = 0;
	philosopher->mutex_dead_exist = 0;
	philosopher->wait_loop_exist = 0;
	philosopher->eat_wait_exist = 0;
	philosopher->eat_finish_exist = 0;
}

int	init_values(t_philosopher **philosopher, int argc, char **argv, int i)
{
	char	*str;
	char	*str_two;

	str = 0;
	str_two = 0;

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
	philosopher[i]->eat_finish_exist = 1;
	free(str_two);
	philosopher[i]->number = i + 1;
	init_values_two(philosopher[i], ft_atoi(argv[1]), argc, argv);
	return (0);
}

int	init_mutex(t_philosopher **philosopher, int nb_philosopher)
{
	sem_t	*mutex;
	sem_t	*sem_eat_wait;
	int	i;

	i = 0;
	mutex = sem_open("mutex1", O_CREAT, S_IRWXU, 1);
	sem_unlink("mutex1");
	while (nb_philosopher > i)
	{
		philosopher[i]->mutex = mutex;
		philosopher[i]->mutex_exist = 1;
		if (philosopher[i]->mutex == SEM_FAILED)
		{
			free_all(philosopher, nb_philosopher);
			return (1);
		}
		i++;
	}
	sem_eat_wait = sem_open("sem_eat_wait", O_CREAT, S_IRWXU, 0);
	sem_unlink("sem_eat_wait");
	i = 0;
	while (nb_philosopher > i)
	{
		philosopher[i]->sem_eat_wait = sem_eat_wait;
		if (philosopher[i]->sem_eat_wait == SEM_FAILED)
		{
			free_all(philosopher, nb_philosopher);
			return (1);
		}
		philosopher[i]->eat_wait_exist = 1;
		i++;
	}
	return (0);
}

int	run_program(t_philosopher **philosopher, char **argv, sem_t *sem_fork, sem_t *sem_dead)
{
	int	result;
	int	i;

	i = 0;
	result = run_process(philosopher, ft_atoi(argv[1]));
	if (result == 2)
	{
		printf("Error\n");
		free_all(philosopher, ft_atoi(argv[1]));
		return (1);
	}
	while (ft_atoi(argv[1]) > i)
	{
		kill(philosopher[i]->process, SIGKILL);
		i++;
	}
	free_all(philosopher, ft_atoi(argv[1]));	
	return (result);
}
static int	loop_set_null(t_philosopher **philo, int nb_philosopher)
{
	int	i;

	i = 0;
	if (!philo)
		return (0);
	while (nb_philosopher > i)
	{
		philo[i] = NULL;
		i++;
	}
	return (1);
}

int	alloc_things(sem_t **sem_fork, sem_t **sem_dead, t_philosopher ***philosopher, char **argv)
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

int	main(int argc, char **argv)
{
	t_philosopher	**philosopher;
	sem_t	*sem_fork;
	sem_t	*sem_dead;
	int	i;
	int	result;

	result = 0;
	i = 0;
	if (check_args(argc, argv) == 1)
		return (1);
	if (check_nb_philosopher(ft_atoi(argv[1])) == 1)
		return (1);
	result = alloc_things(&sem_fork, &sem_dead, &philosopher, argv);
	if (result == 1)
		return (1);
	loop_set_null(philosopher, ft_atoi(argv[1]));
	while (ft_atoi(argv[1]) > i)
	{
		philosopher[i] = malloc(sizeof(t_philosopher));
		if (philosopher[i] == NULL)
		{
			sem_close(sem_fork);
			sem_close(sem_dead);
			result = free_all(philosopher, ft_atoi(argv[1]));
			return (1);
		}
		init_sem_exist(philosopher[i]);
		philosopher[i]->fork = sem_fork;
		philosopher[i]->fork_exist = 1;
		philosopher[i]->mutex_dead = sem_dead;
		philosopher[i]->mutex_dead_exist = 1;
		result = init_values(philosopher, argc, argv, i);
		if (result == 1)
		{
			free_all(philosopher, ft_atoi(argv[1]));
			return (1);
		}
		i++;
	}
	result = check_inputs_values(philosopher, argc, ft_atoi(argv[1]));
	if (result == 1)
		return (1);
	result = init_mutex(philosopher, ft_atoi(argv[1]));
	if (result == 1)
		return (1);
	result = run_program(philosopher, argv, sem_fork, sem_dead);
	return (result);
}
