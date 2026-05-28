#include "philo.h"

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
	if (pthread_mutex_init(&dinner.print_mutex, NULL) != 0)
		return (printf("print mutex init failed.\n"), 1);
	if (!start_thread_test(&dinner))
		return (pthread_mutex_destroy(&dinner.print_mutex), 1);
	pthread_mutex_destroy(&dinner.print_mutex);
	return (0);
}
