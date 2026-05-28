#include "philo.h"

void	log_philo(t_thread_arg *arg, const char *msg)
{
	pthread_mutex_lock(&arg->dinner->print_mutex);
	printf("%ld %d %s\n", elapsed_ms(arg->dinner), arg->id, msg);
	pthread_mutex_unlock(&arg->dinner->print_mutex);
}
