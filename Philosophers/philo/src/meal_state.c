#include "philo.h"

void	update_meal_state(t_philo *philo)
{
	pthread_mutex_lock(&philo->dinner->meal_mutex);
	philo->last_meal_ms = now_ms();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->dinner->meal_mutex);
}

long	get_last_meal(t_philo *philo)
{
	long	value;

	pthread_mutex_lock(&philo->dinner->meal_mutex);
	value = philo->last_meal_ms;
	pthread_mutex_unlock(&philo->dinner->meal_mutex);
	return (value);
}

int	get_meals_eaten(t_philo *philo)
{
	int	value;

	pthread_mutex_lock(&philo->dinner->meal_mutex);
	value = philo->meals_eaten;
	pthread_mutex_unlock(&philo->dinner->meal_mutex);
	return (value);
}
