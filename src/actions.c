#include "philo.h"

static long	get_think_delay(t_philo *philo)
{
	t_rules	*rules;
	long	threshold;
	long	sleep_half;

	rules = &philo->dinner->rules;
	if (rules->n_philo % 2 == 0)
		return (0);
	if (rules->t_die <= rules->t_sleep)
		return (0);
	threshold = rules->t_die - rules->t_sleep;
	threshold = threshold / 2 + threshold % 2;
	if (rules->t_eat >= threshold)
		return (0);
	sleep_half = rules->t_sleep / 2 + rules->t_sleep % 2;
	if (rules->t_eat <= sleep_half)
		return (0);
	return (rules->t_eat - sleep_half);
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
