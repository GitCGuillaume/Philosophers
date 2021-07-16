/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_bonus.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchopin <gchopin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/03 10:14:31 by gchopin           #+#    #+#             */
/*   Updated: 2021/07/16 16:18:44 by gchopin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHER_BONUS_H
  #define PHILOSOPHER_BONUS_H

#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <semaphore.h>
#include "philosopher_tools_bonus.h"
#include <string.h>

typedef struct	s_fork
{
	int	id;
	sem_t	mutex;
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

typedef struct	s_philosopher
{
	t_philosopher_time_state state;
	t_fork	*fork_left;
	t_fork	*fork_right;
	sem_t	*fork;
	sem_t	*mutex;
	sem_t	*secure;
	sem_t	*mutex_dead;
	sem_t	*wait_loop;
	sem_t	*sem_eat_wait;
	sem_t	*sem_eat_finish;
	pthread_t	thread;
	pid_t	process;
	char	fork_exist;
	char	mutex_exist;
	char	secure_exist;
	char	mutex_dead_exist;
	char	wait_loop_exist;
	char	eat_wait_exist;
	char	eat_finish_exist;
	char	dead;
	char	eat;
	char	sleep;
	char	think;
	int	nb_fork;
	int	number;
	int	nb_time_active;
	int	nb_time;
	int	nb_time_reach;
	int	nb_philosopher;
}	t_philosopher;

int	free_all(t_philosopher **philo, int nb_philo);
void	*start_routine(t_philosopher *philosopher);
void	*philo_wait_eat_routine(void *args);
void	*philo_eat_routine(void *args);
void	*philo_dead_routine(void *args);
void	display(t_philosopher *philo, char *str, int is_dead);
int	thinking(t_philosopher *philo);
int	sleeping(t_philosopher *philo);
int	eating(t_philosopher *philo);
int	take_fork(t_philosopher *philo);
int	is_dead(t_philosopher *philo);
int	check_nb_philosopher(int nb_philosopher);
int	check_inputs_values(t_philosopher **philosopher, int argc, int nb_philosopher);
void	stop_routine(t_philosopher *philo, int result);
#endif
