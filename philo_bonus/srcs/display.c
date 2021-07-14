#include "philosopher_bonus.h"

void	display(t_philosopher *philo, char *str, int is_dead)
{
	long int	current_time;
	int	result;

	current_time = math_time();
	if (is_dead == 1)
	{
		printf("%ld %d %s\n",
			current_time - philo->state.start_time, philo->number, str);
		sem_post(philo->wait_loop);
		
	}
	if(is_dead == 0)
	{
		result = sem_wait(philo->mutex_dead);
		printf("%ld %d %s\n",
			philo->state.current_time - philo->state.start_time, philo->number, str);
		sem_post(philo->mutex_dead);
	}
}
