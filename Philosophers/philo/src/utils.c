#include <limits.h>
#include "philo.h"

int	ft_isdigit(int c)
{
	return (c >= '0' && c <= '9');
}

long	ft_atol_positive(const char *str, int *ok)
{
	long	num;
	int		d;

	*ok = 0;
	if (!str || !*str)
		return (0);
	num = 0;
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (0);
		d = *str - '0';
		if (num > (LONG_MAX - d) / 10)
			return (0);
		num = num * 10 + d;
		str++;
	}
	if (num <= 0)
		return (0);
	*ok = 1;
	return (num);
}
