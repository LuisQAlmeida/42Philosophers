#include "philo.h"

long	now_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000L) + (tv.tv_usec / 1000L));
}

long	elapsed_ms(t_dinner *dinner)
{
	return (now_ms() - dinner->start_ms);
}

void	smart_sleep(t_dinner *dinner, long duration_ms)
{
	long	start;
	long	elapsed;

	start = now_ms();
	while (!get_stop(dinner))
	{
		elapsed = now_ms() - start;
		if (elapsed >= duration_ms)
			break ;
		if (duration_ms - elapsed > 1)
			usleep(200);
		else
			usleep(100);
	}
}
