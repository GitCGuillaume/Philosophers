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

static void	loop_routine(t_philosopher *philosopher, int *result)
{
	if (philosopher->nb_fork == 0 && *result == 0
		&& *philosopher->dead == 0 && philosopher->eat == 0
		&& philosopher->sleep == 0
		&& philosopher->nb_philosopher > *philosopher->everyone_eat)
		*result = take_fork(philosopher);
	if (philosopher->nb_fork == 2 && *result == 0
		&& *philosopher->dead == 0 && philosopher->eat == 0
		&& philosopher->sleep == 0
		&& philosopher->nb_philosopher > *philosopher->everyone_eat)
		*result = eating(philosopher);
	if (philosopher->nb_fork == 0 && *result == 0
		&& *philosopher->dead == 0 && philosopher->eat == 1
		&& philosopher->sleep == 0
		&& philosopher->nb_philosopher > *philosopher->everyone_eat)
		*result = sleeping(philosopher);
	if (philosopher->nb_fork == 0 && *result == 0
		&& *philosopher->dead == 0 && philosopher->eat == 1
		&& philosopher->sleep == 1
		&& philosopher->nb_philosopher > *philosopher->everyone_eat)
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

static void run_private_threads(t_philosopher *philosopher)
{
	pthread_create(&philosopher->thread, NULL,
		philo_dead_routine, philosopher);
	pthread_detach(philosopher->thread);
	if (philosopher->nb_time_active == 1)
	{
		pthread_create(&philosopher->eat_thread, NULL,
			philo_eat_routine, philosopher);
		pthread_detach(philosopher->eat_thread);
	}
}

void	*start_routine(void *args)
{
	t_philosopher	*philosopher;
	int				result;

	result = 0;
	philosopher = (t_philosopher *)args;
	run_private_threads(philosopher);
	while (*philosopher->dead == 0
		&& result == 0 && philosopher->nb_philosopher > *philosopher->everyone_eat)
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
	//wait for pthread_detach valgring that is rly slow to terminate correctly
	usleep(15);
	return (NULL);
}
