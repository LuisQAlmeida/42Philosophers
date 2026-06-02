#include "philo.h"

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
