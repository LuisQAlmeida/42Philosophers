#include "philo.h"

void	philo_eat(t_philo *philo)
{
	take_forks(philo);
	log_philo(philo, "is eating");
	smart_sleep(philo->dinner, philo->dinner->rules.t_eat);
	drop_forks(philo);
}

void	philo_sleep(t_philo *philo)
{
	log_philo(philo, "is sleeping");
	smart_sleep(philo->dinner, philo->dinner->rules.t_sleep);
}

void	philo_think(t_philo *philo)
{
	log_philo(philo, "is thinking");
}
