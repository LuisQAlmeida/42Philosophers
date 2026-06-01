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
	return (1);
}

static void	destroy_dinner_mutexes(t_dinner *dinner)
{
	pthread_mutex_destroy(&dinner->state_mutex);
	pthread_mutex_destroy(&dinner->print_mutex);
}

int	main(int argc, char **argv)
{
	t_dinner	dinner;

	memset(&dinner, 0, sizeof(dinner));
	if (!parse_rules(&dinner.rules, argc, argv))
	{
		printf("Invalid arguments.\n");
		printf("Usage: ./philo n_philo t_die t_eat t_sleep [must_eat]\n");
		return (1);
	}
	dinner.start_ms = now_ms();
	if (!init_dinner_mutexes(&dinner))
		return (printf("mutex init failed.\n"), 1);
	if (!init_forks(&dinner))
	{
		destroy_dinner_mutexes(&dinner);
		return (printf("fork init failed.\n"), 1);
	}
	if (!start_thread_test(&dinner))
	{
		destroy_forks(&dinner, dinner.rules.n_philo);
		destroy_dinner_mutexes(&dinner);
		return (1);
	}
	destroy_forks(&dinner, dinner.rules.n_philo);
	destroy_dinner_mutexes(&dinner);
	return (0);
}
