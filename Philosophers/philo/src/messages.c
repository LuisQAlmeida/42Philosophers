#include "philo.h"

int	print_error(const char *msg, int value)
{
	printf("%s\n", msg);
	return (value);
}

int	print_usage(void)
{
	printf("Invalid arguments.\n");
	printf("Usage: ./philo n_philo t_die t_eat t_sleep [must_eat]\n");
	return (1);
}
