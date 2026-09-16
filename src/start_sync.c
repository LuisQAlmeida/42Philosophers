#include "philo.h"

static void	mark_philo_ready(t_dinner *dinner)
{
	pthread_mutex_lock(&dinner->start_mutex);
	dinner->ready_count++;
	pthread_mutex_unlock(&dinner->start_mutex);
}

int	all_philos_ready(t_dinner *dinner)
{
	int	ready;

	pthread_mutex_lock(&dinner->start_mutex);
	ready = (dinner->ready_count == dinner->rules.n_philo);
	pthread_mutex_unlock(&dinner->start_mutex);
	return (ready);
}

static int	get_start(t_dinner *dinner)
{
	int	value;

	pthread_mutex_lock(&dinner->start_mutex);
	value = dinner->start;
	pthread_mutex_unlock(&dinner->start_mutex);
	return (value);
}

void	wait_for_start(t_philo *philo)
{
	mark_philo_ready(philo->dinner);
	while (!get_start(philo->dinner) && !get_stop(philo->dinner))
		usleep(100);
}

void	release_start(t_dinner *dinner)
{
	int	i;

	pthread_mutex_lock(&dinner->meal_mutex);
	dinner->start_ms = now_ms();
	i = 0;
	while (i < dinner->rules.n_philo)
	{
		dinner->philos[i].last_meal_ms = dinner->start_ms;
		i++;
	}
	pthread_mutex_unlock(&dinner->meal_mutex);
	pthread_mutex_lock(&dinner->start_mutex);
	dinner->start = 1;
	pthread_mutex_unlock(&dinner->start_mutex);
}
