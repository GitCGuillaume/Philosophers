/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchopin <gchopin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/23 11:25:41 by gchopin           #+#    #+#             */
/*   Updated: 2021/07/15 11:46:29 by gchopin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHER_H
# define PHILOSOPHER_H

#include <pthread.h>
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "philosopher_tools.h"

typedef struct	s_fork
{
	int	id;
	char	fork_exist;
	pthread_mutex_t	mutex;
}	t_fork;

typedef struct	s_philosopher_time_state
{
	long int	current_time;
	long int	start_time;
	long int	time_simulation;
	long int	time_to_die;
	long int	time_to_eat;
	long int	time_to_sleep;
	long int	nb_time_eat;
}	t_philosopher_time_state;

typedef struct s_philosopher
{
	t_philosopher_time_state state;
	t_fork	*fork_right;
	t_fork	*fork_left;
	pthread_t thread;
	pthread_t main_thread;
	pthread_t eat_thread;
	pthread_mutex_t secure;
	pthread_mutex_t	mutex;
	pthread_mutex_t	mutex_eat;
	int	*dead;
	int	*everyone_eat;
	char	secure_exist;
	char	mutex_exist;
	char	mutex_eat_exist;
	int	number;
	int	eat;
	int	sleep;
	int	nb_fork;
	int	think;
	int	nb_time_active;
	int	nb_time;
	int	nb_time_reach;
	int	nb_philosopher;
} t_philosopher;

typedef struct s_main
{
	pthread_t thread;
	int	dead;
	int	eat_at_least;
} t_main;

/*
 ** THREAD
*/
void	*start_routine(void *args);
int	is_dead(t_philosopher *philo);
int	thinking(t_philosopher *philo);
int	sleeping(t_philosopher *philo);
int	eating(t_philosopher *philo);
int	take_fork(t_philosopher *philo);
void	*philo_dead_routine(void *args);
void	*philo_eat_routine(void *args);
int	running_thread(t_philosopher **philosopher, int argc, int nb_philosopher);

/*
 ** FREE
*/

void	free_all(t_philosopher **philo, t_fork **fork, int nb_philo);
void	free_init_null(t_philosopher **philo, t_fork **fork, int nb_philosopher);

/*
 ** TOOLS
*/
int	check_inputs_values(t_philosopher **philosopher, t_fork **fork,
	int argc, int nb_philosopher);
int	check_nb_philosopher(int nb_philosopher);
/*
 ** INIT
*/
int	init_to_null(t_philosopher **philo, t_fork **fork, int nb_philosopher);
int	init_values(t_philosopher **philosopher, t_fork **fork, char **argv, int argc);
void	init_values_two(t_philosopher **philosopher, char **argv, int argc, int i);
int	init_forks(t_philosopher **philosopher, t_fork **fork, int nb_philosopher);
int	init_mutex(t_philosopher **philosopher, int nb_philosopher);

#endif
