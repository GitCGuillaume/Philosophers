/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchopin <gchopin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/23 11:25:41 by gchopin           #+#    #+#             */
/*   Updated: 2021/07/01 19:28:44 by gchopin          ###   ########.fr       */
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
	pthread_mutex_t secure;
	pthread_mutex_t	mutex;
	int	*dead;
	int	number;
	int	eat;
	int	sleep;
	int	nb_fork;
	int	think;
	int	nb_time_active;
	int	nb_time;
	int	nb_time_reach;
	int	everyone;
} t_philosopher;

typedef struct s_main
{
	pthread_t thread;
	int	dead;
	int	eat_at_least;
} t_main;

#endif
