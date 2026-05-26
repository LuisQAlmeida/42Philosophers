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
	printf("n_philo = %d | t_die = %ld | t_eat = %ld | t_sleep = %ld ",
		rules.n_philo, rules.t_die, rules.t_eat, rules.t_sleep);
	if (rules.has_must_eat)
		printf("| must_eat = %d\n", rules.must_eat);
	else
		printf("\n");
	return (0);
}
