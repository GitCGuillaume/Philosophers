#include "philosopher.h"
/*
static void	dead_routine_two(t_philosopher *philo)
{
	if (philo->fork_left->fork_exist == 1)
	{
		philo->fork_left->fork_exist = 2;
		pthread_mutex_unlock(&philo->fork_right->mutex);
		pthread_mutex_unlock(&philo->fork_left->mutex);
	}
}*/

static void	*philo_dead_routine(void *args)
{
	t_philosopher	*philo;
	int				result;

	philo = (t_philosopher *)args;
	result = 0;
	pthread_mutex_lock(&philo->mutex);
	while (result == 0 && *philo->dead == 0)
	{
		/*if (philo->nb_time_reach > 0)
		{
			pthread_mutex_unlock(&philo->mutex);
			return (NULL);
		}*/
		result = is_dead(philo);
		if (result == 1)
		{
			//dead_routine_two(philo);		
			return (NULL);
		}
		usleep(10);
	}
	pthread_mutex_unlock(&philo->mutex);
	return (NULL);
}

static void	loop_routine(t_philosopher *philosopher, int *result)
{
	if (philosopher->nb_fork == 0 && *result == 0
		&& *philosopher->dead == 0 && philosopher->eat == 0
		&& philosopher->sleep == 0)
		*result = take_fork(philosopher);
	if (philosopher->nb_fork == 2 && *result == 0
		&& *philosopher->dead == 0 && philosopher->eat == 0
		&& philosopher->sleep == 0)
		*result = eating(philosopher);
	if (philosopher->nb_fork == 0 && *result == 0
		&& *philosopher->dead == 0 && philosopher->eat == 1
		&& philosopher->sleep == 0)
		*result = sleeping(philosopher);
	if (philosopher->nb_fork == 0 && *result == 0
		&& *philosopher->dead == 0 && philosopher->eat == 1
		&& philosopher->sleep == 1)
		*result = thinking(philosopher);
}

/*
 ** anti deadlock when die
*/

static void	close_routine(t_philosopher *philosopher, int *result)
{
	if (philosopher->fork_left->fork_exist == 1
		/*&& philosopher->eat == 0*/ && *result == 0)
	{
		philosopher->fork_left->fork_exist = 2;
		pthread_mutex_unlock(&philosopher->fork_right->mutex);
		pthread_mutex_unlock(&philosopher->fork_left->mutex);
	}
}

void	*start_routine(void *args)
{
	t_philosopher	*philosopher;
	int				result;

	result = 0;
	philosopher = (t_philosopher *)args;
	pthread_create(&philosopher->thread, NULL,
		philo_dead_routine, philosopher);
	pthread_detach(philosopher->thread);
	while (*philosopher->dead == 0 && philosopher->nb_time_reach == 0
		&& result == 0)
	{
		loop_routine(philosopher, &result);
		if (result != 0 && philosopher->fork_left->fork_exist == 1
			&& philosopher->eat == 0)
		{
			*philosopher->dead = 1;
			philosopher->fork_left->fork_exist = 2;
			pthread_mutex_unlock(&philosopher->fork_right->mutex);
			pthread_mutex_unlock(&philosopher->fork_left->mutex);
			break;
		}
	}
	close_routine(philosopher, &result);
	//wait for pthread_detach to terminate correctly
	usleep(5);
	//printf("a\n");
	return (NULL);
}
