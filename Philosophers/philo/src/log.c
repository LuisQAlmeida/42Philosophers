#include "philo.h"

void	log_philo(t_philo *philo, const char *msg)
{
	pthread_mutex_lock(&philo->dinner->print_mutex);
	if (!get_stop(philo->dinner) || ft_strcmp(msg, "died") == 0)
		printf("%ld %d %s\n", elapsed_ms(philo->dinner), philo->id, msg);
	pthread_mutex_unlock(&philo->dinner->print_mutex);
}
