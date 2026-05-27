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
	if (!start_thread_test(&dinner))
		return (1);
	return (0);
}
