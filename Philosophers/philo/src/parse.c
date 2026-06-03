#include "philo.h"

static int	set_long(long *dst, const char *src)
{
	int		ok;
	long	value;

	value = ft_atol_positive(src, &ok);
	if (!ok)
		return (0);
	*dst = value;
	return (1);
}

static int	set_int(int *dst, const char *src)
{
	int		ok;
	long	value;

	value = ft_atol_positive(src, &ok);
	if (!ok || value > INT_MAX)
		return (0);
	*dst = (int)value;
	return (1);
}

int	parse_rules(t_rules *rules, int ac, char **av)
{
	memset(rules, 0, sizeof(*rules));
	if (ac != 5 && ac != 6)
		return (0);
	if (!set_int(&rules->n_philo, av[1]))
		return (0);
	if (!set_long(&rules->t_die, av[2]))
		return (0);
	if (!set_long(&rules->t_eat, av[3]))
		return (0);
	if (!set_long(&rules->t_sleep, av[4]))
		return (0);
	if (ac == 6)
	{
		rules->has_must_eat = 1;
		if (!set_int(&rules->must_eat, av[5]))
			return (0);
	}
	return (1);
}
