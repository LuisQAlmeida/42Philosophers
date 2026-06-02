#include "philo.h"

static void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->dinner->rules.n_philo == 1)
	{
		handle_single_philo(philo);
		return (NULL);
	}
	while (!get_stop(philo->dinner))
	{
		philo_eat(philo);
		philo_sleep(philo);
		philo_think(philo);
	}
	return (NULL);
}

static void	join_philos(t_dinner *dinner, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		pthread_join(dinner->philos[i].thread, NULL);
		i++;
	}
}

static void	init_philos(t_dinner *dinner)
{
	int	i;

	i = 0;
	while (i < dinner->rules.n_philo)
	{
		dinner->philos[i].id = i + 1;
		dinner->philos[i].left_fork = i;
		dinner->philos[i].right_fork = (i + 1) % dinner->rules.n_philo;
		dinner->philos[i].meals_eaten = 0;
		dinner->philos[i].last_meal_ms = dinner->start_ms;
		dinner->philos[i].dinner = dinner;
		i++;
	}
}

static int	create_philos(t_dinner *dinner)
{
	int	i;

	i = 0;
	while (i < dinner->rules.n_philo)
	{
		if (pthread_create(&dinner->philos[i].thread, NULL,
				philo_routine, &dinner->philos[i]) != 0)
		{
			join_philos(dinner, i);
			return (0);
		}
		i++;
	}
	return (1);
}

int	start_thread_test(t_dinner *dinner)
{
	dinner->philos = malloc(sizeof(t_philo) * dinner->rules.n_philo);
	if (!dinner->philos)
		return (printf("malloc failed.\n"), 0);
	memset(dinner->philos, 0, sizeof(t_philo) * dinner->rules.n_philo);
	init_philos(dinner);
	if (!create_philos(dinner))
	{
		free(dinner->philos);
		dinner->philos = NULL;
		return (printf("pthread_create failed.\n"), 0);
	}
	monitor_dinner(dinner);
	join_philos(dinner, dinner->rules.n_philo);
	free(dinner->philos);
	dinner->philos = NULL;
	return (1);
}
