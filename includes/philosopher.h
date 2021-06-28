/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gchopin <gchopin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/23 11:25:41 by gchopin           #+#    #+#             */
/*   Updated: 2021/06/28 18:01:04 by gchopin          ###   ########.fr       */
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
	long int	time_simulation;
	long int	time_to_die;
	long int	time_to_eat;
	long int	time_to_sleep;
	long int	nb_time_eat;
	long int	nb_time;
}	t_philosopher_time_state;

typedef struct s_philosopher
{
	t_philosopher_time_state state;
	t_fork	*fork_right;
	t_fork	*fork_left;
	pthread_t thread;
	int	number;
	int	eat;
	int	sleep;
	int	dead;
	int	nb_fork;
	int	think;
} t_philosopher;

#endif
