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
	long	end;
	long	remaining;

	end = now_ms() + duration_ms;
	while (!get_stop(dinner))
	{
		remaining = end - now_ms();
		if (remaining <= 0)
			break ;
		if (remaining > 1)
			usleep(200);
		else
			usleep(100);
	}
}
