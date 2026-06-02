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

void	monitor_dinner(t_dinner *dinner)
{
	while (!get_stop(dinner))
	{
		if (check_deaths(dinner))
			return ;
		usleep(500);
	}
}
