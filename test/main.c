/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchopin <gchopin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/22 15:19:51 by gchopin           #+#    #+#             */
/*   Updated: 2021/06/25 13:25:55 by gchopin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct	s_fork
{
	pthread_mutex_t	fork;
}	t_fork;

typedef struct s_data
{
	int	data;
}	t_data;

typedef struct	s_thread
{
	pthread_t	thread;
}	t_thread;

void	*start_routine(void *args)
{
	t_data	*data;

	data = (t_data *)args;
	return (NULL);
}

void	*threading_process(t_data *data)
{
	t_thread	*philo;
	t_fork		**fork;

	fork = malloc(sizeof(t_fork) * 2);
	if (fork==NULL)
		return(0);
	//fork[0] = malloc(sizeof(pthread_mutex_t));
	//if (fork[0]==NULL)
	//	return(0);
	philo = malloc(sizeof(philo));
	if (philo==NULL)
		return(0);
	fork[0] = malloc(sizeof(pthread_mutex_t));
	if (fork[0]==NULL)
		return(0);
//	*fork = malloc(sizeof(*fork));
//	if (*fork==NULL)
//		return(0);
	pthread_create(&philo->thread, NULL, start_routine, data);
	usleep(30);
	pthread_mutex_init(&fork[0]->fork, NULL);
	//usleep(30);
	//pthread_mutex_init(&fork[1]->fork, NULL);
	usleep(30);
	//pthread_mutex_lock(&fork[0]->fork);
	//usleep(30);
	//pthread_mutex_unlock(&fork[1]->fork);
	
	//usleep(30);
	int test;
	test = pthread_mutex_destroy(&fork[0]->fork);
	usleep(30);
	printf("test=%d\n", test);
	usleep(30);
	//pthread_mutex_destroy(&fork[1]->fork);
	usleep(60);
	pthread_detach(philo->thread);
	free(philo);
	free(fork[0]);
	//free(fork[1]);
	free(fork);
	return (NULL);
}
int	main(void)
{
	t_data		*data;

	data = malloc(sizeof(data));
	if (data==NULL)
		return(0);
	data->data=1;
	threading_process(data);
	free(data);
	printf("timeval sizeof=%lu", sizeof(struct timeval));
	return (0);
}
