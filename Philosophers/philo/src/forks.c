#include "philo.h"

void	destroy_forks(t_dinner *dinner, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		pthread_mutex_destroy(&dinner->forks[i]);
		i++;
	}
	free(dinner->forks);
	dinner->forks = NULL;
}

int	init_forks(t_dinner *dinner)
{
	int	i;

	dinner->forks = malloc(sizeof(pthread_mutex_t) * dinner->rules.n_philo);
	if (!dinner->forks)
		return (0);
	i = 0;
	while (i < dinner->rules.n_philo)
	{
		if (pthread_mutex_init(&dinner->forks[i], NULL) != 0)
		{
			destroy_forks(dinner, i);
			return (0);
		}
		i++;
	}
	return (1);
}

static void	lock_fork(t_philo *philo, int fork_id)
{
	pthread_mutex_lock(&philo->dinner->forks[fork_id]);
	log_philo(philo, "has taken a fork");
}

void	take_forks(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		lock_fork(philo, philo->right_fork);
		lock_fork(philo, philo->left_fork);
	}
	else
	{
		lock_fork(philo, philo->left_fork);
		lock_fork(philo, philo->right_fork);
	}
}

void	drop_forks(t_philo *philo)
{
	pthread_mutex_unlock(&philo->dinner->forks[philo->left_fork]);
	pthread_mutex_unlock(&philo->dinner->forks[philo->right_fork]);
}
