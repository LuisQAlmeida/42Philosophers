#include "philo.h"

void	philo_eat(t_philo *philo)
{
	if (get_stop(philo->dinner))
		return ;
	take_forks(philo);
	if (get_stop(philo->dinner))
	{
		drop_forks(philo);
		return ;
	}
	update_meal_state(philo);
	log_philo(philo, "is eating");
	smart_sleep(philo->dinner, philo->dinner->rules.t_eat);
	drop_forks(philo);
}

void	philo_sleep(t_philo *philo)
{
	if (get_stop(philo->dinner))
		return ;
	log_philo(philo, "is sleeping");
	smart_sleep(philo->dinner, philo->dinner->rules.t_sleep);
}

void	philo_think(t_philo *philo)
{
	if (get_stop(philo->dinner))
		return ;
	log_philo(philo, "is thinking");
}

void	handle_single_philo(t_philo *philo)
{
	pthread_mutex_lock(&philo->dinner->forks[philo->left_fork]);
	log_philo(philo, "has taken a fork");
	smart_sleep(philo->dinner, philo->dinner->rules.t_die);
	pthread_mutex_unlock(&philo->dinner->forks[philo->left_fork]);
}
