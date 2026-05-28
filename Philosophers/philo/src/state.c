#include "philo.h"

void	set_stop(t_dinner *dinner, int value)
{
	pthread_mutex_lock(&dinner->state_mutex);
	dinner->stop = value;
	pthread_mutex_unlock(&dinner->state_mutex);
}

int	get_stop(t_dinner *dinner)
{
	int	value;

	pthread_mutex_lock(&dinner->state_mutex);
	value = dinner->stop;
	pthread_mutex_unlock(&dinner->state_mutex);
	return (value);
}
