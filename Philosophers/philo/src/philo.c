#include "philo.h"

int	main(int argc, char **argv)
{
	t_rules	rules;

	if (!parse_rules(&rules, argc, argv))
	{
		printf("Invalid arguments.\n");
		printf("Usage: ./philo n_philo t_die t_eat t_sleep [must_eat]\n");
		return (1);
	}
	if (!start_thread_test(&rules))
		return (1);
	return (0);
}
