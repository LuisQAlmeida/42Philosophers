#include "philo.h"

static void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	wait_for_start(philo);
	if (get_stop(philo->dinner))
		return (NULL);
	if (philo->dinner->rules.n_philo == 1)
	{
		handle_single_philo(philo);
		return (NULL);
	}
	if (philo->id % 2 == 0)
		smart_sleep(philo->dinner, philo->dinner->rules.t_eat / 2);
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
		dinner->philos[i].last_meal_ms = 0;
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
			set_stop(dinner, 1);
			join_philos(dinner, i);
			return (0);
		}
		i++;
	}
	return (1);
}

int	start_simulation(t_dinner *dinner)
{
	size_t	bytes;

	bytes = sizeof(t_philo) * (size_t)dinner->rules.n_philo;
	dinner->philos = malloc(bytes);
	if (!dinner->philos)
		return (print_error("malloc failed.", 0));
	memset(dinner->philos, 0, bytes);
	init_philos(dinner);
	if (!create_philos(dinner))
	{
		free(dinner->philos);
		dinner->philos = NULL;
		return (print_error("pthread_create failed.", 0));
	}
	while (!all_philos_ready(dinner))
		usleep(100);
	release_start(dinner);
	monitor_dinner(dinner);
	join_philos(dinner, dinner->rules.n_philo);
	free(dinner->philos);
	dinner->philos = NULL;
	return (1);
}
