#include "philo.h"

int	is_philo_dead(t_philo *philo)
{
	long	last_meal;
	long	now;

	last_meal = get_last_meal(philo);
	now = now_ms();
	if (now - last_meal > philo->dinner->rules.t_die)
		return (1);
	return (0);
}

static int	check_deaths(t_dinner *dinner)
{
	int	i;

	i = 0;
	while (i < dinner->rules.n_philo)
	{
		if (is_philo_dead(&dinner->philos[i]))
		{
			set_stop(dinner, 1);
			log_philo(&dinner->philos[i], "died");
			return (1);
		}
		i++;
	}
	return (0);
}

int	enough_meals_eaten(t_dinner *dinner)
{
	int	i;

	if (!dinner->rules.has_must_eat)
		return (0);
	i = 0;
	while (i < dinner->rules.n_philo)
	{
		if (get_meals_eaten(&dinner->philos[i]) < dinner->rules.must_eat)
			return (0);
		i++;
	}
	return (1);
}

void	monitor_dinner(t_dinner *dinner)
{
	while (!get_stop(dinner))
	{
		if (check_deaths(dinner))
			return ;
		if (enough_meals_eaten(dinner))
		{
			set_stop(dinner, 1);
			return ;
		}
		usleep(200);
	}
}
