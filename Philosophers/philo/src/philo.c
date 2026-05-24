#include "philo.h"

int	main(int argc, char **argv)
{
	if (!parse_args(argc, argv))
	{
		printf("Invalid arguments.\n");
		printf("Usage: ./philo n_philo t_die t_eat t_sleep [must_eat]\n");
		return (1);
	}
	printf("Valid arguments inserted: positive integers.\n");
	return (0);
}
