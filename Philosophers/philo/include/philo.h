#ifndef PHILO_H
# define PHILO_H

# include <limits.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

/* ************************************************************************** */
/*                                Structures                                  */
/* ************************************************************************** */

typedef struct s_dinner	t_dinner;

typedef struct s_rules
{
	int		n_philo;
	long	t_die;
	long	t_eat;
	long	t_sleep;
	int		must_eat;
	int		has_must_eat;
}	t_rules;

typedef struct s_philo
{
	int			id;
	int			left_fork;
	int			right_fork;
	int			meals_eaten;
	long		last_meal_ms;
	pthread_t	thread;
	t_dinner	*dinner;
}	t_philo;

typedef struct s_dinner
{
	t_rules			rules;
	long			start_ms;
	int				start;
	int				stop;
	int				ready_count;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	state_mutex;
	pthread_mutex_t	meal_mutex;
	pthread_mutex_t	start_mutex;
	pthread_mutex_t	*forks;
	t_philo			*philos;
}	t_dinner;

/* ************************************************************************** */
/*                                  Parsing                                   */
/* ************************************************************************** */

int		parse_rules(t_rules *rules, int ac, char **av);

/* ************************************************************************** */
/*                                Simulation                                  */
/* ************************************************************************** */

void	wait_for_start(t_philo *philo);
void	release_start(t_dinner *dinner);
int		all_philos_ready(t_dinner *dinner);
int		start_simulation(t_dinner *dinner);

/* ************************************************************************** */
/*                                  Actions                                   */
/* ************************************************************************** */

void	philo_eat(t_philo *philo);
void	philo_sleep(t_philo *philo);
void	philo_think(t_philo *philo);
void	handle_single_philo(t_philo *philo);

/* ************************************************************************** */
/*                                   Forks                                    */
/* ************************************************************************** */

int		init_forks(t_dinner *dinner);
void	destroy_forks(t_dinner *dinner, int count);
void	take_forks(t_philo *philo);
void	drop_forks(t_philo *philo);

/* ************************************************************************** */
/*                                Meal State                                  */
/* ************************************************************************** */

void	update_meal_state(t_philo *philo);
int		stop_if_starved(t_philo *philo);
int		get_meals_eaten(t_philo *philo);

/* ************************************************************************** */
/*                                  Monitor                                   */
/* ************************************************************************** */

void	monitor_dinner(t_dinner *dinner);
int		is_philo_dead(t_philo *philo);
int		enough_meals_eaten(t_dinner *dinner);

/* ************************************************************************** */
/*                                   State                                    */
/* ************************************************************************** */

void	set_stop(t_dinner *dinner, int value);
int		get_stop(t_dinner *dinner);

/* ************************************************************************** */
/*                                    Time                                    */
/* ************************************************************************** */

long	now_ms(void);
long	elapsed_ms(t_dinner *dinner);
void	smart_sleep(t_dinner *dinner, long duration_ms);

/* ************************************************************************** */
/*                                   Output                                   */
/* ************************************************************************** */

void	log_philo(t_philo *philo, const char *msg);
int		print_error(const char *msg, int value);
int		print_usage(void);

/* ************************************************************************** */
/*                                   Utils                                    */
/* ************************************************************************** */

int		ft_isdigit(int c);
long	ft_atol_positive(const char *s, int *ok);
int		ft_strcmp(const char *s1, const char *s2);

#endif
