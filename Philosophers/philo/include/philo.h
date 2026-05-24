#ifndef PHILO_H
# define PHILO_H

# include <string.h> //memset
# include <stdio.h> //printf
# include <stdlib.h> //malloc, free
# include <unistd.h> //write, usleep
# include <sys/time.h> //gettimeofday
# include <pthread.h> /*p_thread_create, pthread_detach, pthread_join, pthread_mutex_init,
			p_thread_mutex_destroy, pthread_mutex_lock, pthread_mutex_unlock*/

int		ft_isdigit(int c);
long	ft_atol_positive(const char *s, int *ok);
int		parse_args(int ac, char **av);

#endif
