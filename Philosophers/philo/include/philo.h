#ifndef PHILO_H
# define PHILO_H

# include <string.h> //memset
# include <stdio.h> //printf
# include <stdlib.h> //malloc, free
# include <unistd.h> //write, usleep
# include <sys/time.h> //gettimeofday
# include <pthread.h> /*p_thread_create, pthread_detach, pthread_join, pthread_mutex_init,
			p_thread_mutex_destroy, pthread_mutex_lock, pthread_mutex_unlock*/

typedef struct s_rules
{
	int		n_philo;
	long	t_die;
	long	t_eat;
	long	t_sleep;
	int		must_eat;
	int		has_must_eat;
}	t_rules;

typedef struct s_dinner	t_dinner;

typedef struct s_philo
{
	int			id;
	int			left_fork;
	int			right_fork;
	pthread_t	thread;
	t_dinner	*dinner;
}	t_philo;

typedef struct s_dinner
{
	t_rules			rules;
	long			start_ms;
	int				stop;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	state_mutex;
	pthread_mutex_t	*forks;
	t_philo			*philos;
}	t_dinner;

int		ft_isdigit(int c);
long	ft_atol_positive(const char *s, int *ok);
int		parse_rules(t_rules *rules, int ac, char **av);
int		start_thread_test(t_dinner *dinner);
long	now_ms(void);
long	elapsed_ms(t_dinner *dinner);
void	log_philo(t_philo *philo, const char *msg);
void	set_stop(t_dinner *dinner, int value);
int		get_stop(t_dinner *dinner);
int		init_forks(t_dinner *dinner);
void	destroy_forks(t_dinner *dinner, int count);
void	take_forks(t_philo *philo);
void	drop_forks(t_philo *philo);

#endif
