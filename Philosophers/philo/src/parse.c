#include "philo.h"

int	parse_args(int ac, char **av)
{
	int		i;
	int		ok;
	long	value;

	if (ac != 5 && ac != 6)
		return (0);
	i = 1;
	while (i < ac)
	{
		value = ft_atol_positive(av[i], &ok);
		if (!ok || value <= 0)
			return (0);
		i++;
	}
	return (1);
}
