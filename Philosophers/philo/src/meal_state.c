#include "philo.h"

void	update_meal_state(t_philo *philo)
{
	pthread_mutex_lock(&philo->dinner->meal_mutex);
	philo->last_meal_ms = now_ms();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->dinner->meal_mutex);
}

int	stop_if_starved(t_philo *philo)
{
	int		dead;
	long	now;

	pthread_mutex_lock(&philo->dinner->meal_mutex);
	now = now_ms();
	dead = (now - philo->last_meal_ms
			>= philo->dinner->rules.t_die);
	if (dead)
		set_stop(philo->dinner, 1);
	pthread_mutex_unlock(&philo->dinner->meal_mutex);
	return (dead);
}

int	get_meals_eaten(t_philo *philo)
{
	int	value;

	pthread_mutex_lock(&philo->dinner->meal_mutex);
	value = philo->meals_eaten;
	pthread_mutex_unlock(&philo->dinner->meal_mutex);
	return (value);
}
