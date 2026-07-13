#include "philo.h"

static long	get_think_delay(t_philo *philo)
{
	long	delay;
	long	min_cycle;

	delay = 0;
	if (philo->dinner->rules.n_philo % 2 == 0)
		return (0);
	min_cycle = philo->dinner->rules.t_eat * 2;
	min_cycle += philo->dinner->rules.t_sleep;
	if (philo->dinner->rules.t_die <= min_cycle)
		return (0);
	delay = philo->dinner->rules.t_eat * 2;
	delay -= philo->dinner->rules.t_sleep;
	if (delay < 0)
		delay = 0;
	delay /= 2;
	return (delay);
}

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
	long	delay;

	if (get_stop(philo->dinner))
		return ;
	log_philo(philo, "is thinking");
	delay = get_think_delay(philo);
	if (delay > 0)
		smart_sleep(philo->dinner, delay);
}

void	handle_single_philo(t_philo *philo)
{
	pthread_mutex_lock(&philo->dinner->forks[philo->left_fork]);
	log_philo(philo, "has taken a fork");
	smart_sleep(philo->dinner, philo->dinner->rules.t_die);
	pthread_mutex_unlock(&philo->dinner->forks[philo->left_fork]);
}
