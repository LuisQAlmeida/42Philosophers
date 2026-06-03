#include "philo.h"

static int	init_dinner_mutexes(t_dinner *dinner)
{
	if (pthread_mutex_init(&dinner->print_mutex, NULL) != 0)
		return (0);
	if (pthread_mutex_init(&dinner->state_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&dinner->print_mutex);
		return (0);
	}
	if (pthread_mutex_init(&dinner->meal_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&dinner->state_mutex);
		pthread_mutex_destroy(&dinner->print_mutex);
		return (0);
	}
	return (1);
}

static void	destroy_dinner_mutexes(t_dinner *dinner)
{
	pthread_mutex_destroy(&dinner->meal_mutex);
	pthread_mutex_destroy(&dinner->state_mutex);
	pthread_mutex_destroy(&dinner->print_mutex);
}

static int	start_dinner(t_dinner *dinner)
{
	if (!init_dinner_mutexes(dinner))
		return (print_error("mutex init failed.", 1));
	if (!init_forks(dinner))
	{
		destroy_dinner_mutexes(dinner);
		return (print_error("fork init failed.", 1));
	}
	if (!start_simulation(dinner))
	{
		destroy_forks(dinner, dinner->rules.n_philo);
		destroy_dinner_mutexes(dinner);
		return (1);
	}
	destroy_forks(dinner, dinner->rules.n_philo);
	destroy_dinner_mutexes(dinner);
	return (0);
}

int	main(int argc, char **argv)
{
	t_dinner	dinner;

	memset(&dinner, 0, sizeof(dinner));
	if (!parse_rules(&dinner.rules, argc, argv))
		return (print_usage());
	dinner.start_ms = now_ms();
	return (start_dinner(&dinner));
}
