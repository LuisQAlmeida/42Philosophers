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

/**
 * @brief Stores the simulation rules parsed from command-line arguments.
 *
 * All time values are stored in milliseconds.
 */
typedef struct s_rules
{
	int		n_philo;
	long	t_die;
	long	t_eat;
	long	t_sleep;
	int		must_eat;
	int		has_must_eat;
}	t_rules;

/**
 * @brief Represents one philosopher and its runtime state.
 *
 * Each philosopher stores its fork indexes, meal information, thread handle,
 * and a pointer to the shared dinner state.
 */
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

/**
 * @brief Shared simulation state.
 *
 * Contains the global rules, start timestamp, stop flag, synchronization
 * mutexes, fork mutexes, and philosopher array.
 */
typedef struct s_dinner
{
	t_rules			rules;
	long			start_ms;
	int				stop;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	state_mutex;
	pthread_mutex_t	meal_mutex;
	pthread_mutex_t	*forks;
	t_philo			*philos;
}	t_dinner;

/* ************************************************************************** */
/*                                  Parsing                                   */
/* ************************************************************************** */

/**
 * @brief Parses and validates command-line arguments into simulation rules.
 *
 * @param rules Destination structure.
 * @param ac Argument count.
 * @param av Argument vector.
 * @return 1 on success, 0 on invalid input.
 */
int		parse_rules(t_rules *rules, int ac, char **av);

/* ************************************************************************** */
/*                                Simulation                                  */
/* ************************************************************************** */

/**
 * @brief Allocates philosophers, starts threads, runs the monitor and cleanup.
 *
 * @param dinner Shared simulation state.
 * @return 1 on success, 0 on failure.
 */
int		start_simulation(t_dinner *dinner);

/* ************************************************************************** */
/*                                  Actions                                   */
/* ************************************************************************** */

/**
 * @brief Executes the eating phase for one philosopher.
 */
void	philo_eat(t_philo *philo);

/**
 * @brief Executes the sleeping phase for one philosopher.
 */
void	philo_sleep(t_philo *philo);

/**
 * @brief Executes the thinking phase for one philosopher.
 */
void	philo_think(t_philo *philo);

/**
 * @brief Handles the special case where there is only one philosopher.
 */
void	handle_single_philo(t_philo *philo);

/* ************************************************************************** */
/*                                   Forks                                    */
/* ************************************************************************** */

/**
 * @brief Allocates and initializes all fork mutexes.
 *
 * @return 1 on success, 0 on failure.
 */
int		init_forks(t_dinner *dinner);

/**
 * @brief Destroys initialized fork mutexes and frees the fork array.
 */
void	destroy_forks(t_dinner *dinner, int count);

/**
 * @brief Locks both forks needed by a philosopher.
 */
void	take_forks(t_philo *philo);

/**
 * @brief Unlocks both forks held by a philosopher.
 */
void	drop_forks(t_philo *philo);

/* ************************************************************************** */
/*                                Meal State                                  */
/* ************************************************************************** */

/**
 * @brief Updates a philosopher's last meal timestamp and meal counter.
 */
void	update_meal_state(t_philo *philo);

/**
 * @brief Safely reads a philosopher's last meal timestamp.
 */
long	get_last_meal(t_philo *philo);

/**
 * @brief Safely reads a philosopher's meal counter.
 */
int		get_meals_eaten(t_philo *philo);

/* ************************************************************************** */
/*                                  Monitor                                   */
/* ************************************************************************** */

/**
 * @brief Runs the monitoring loop for death and meal completion.
 */
void	monitor_dinner(t_dinner *dinner);

/**
 * @brief Checks whether a philosopher exceeded time_to_die.
 *
 * @return 1 if dead, 0 otherwise.
 */
int		is_philo_dead(t_philo *philo);

/**
 * @brief Checks whether all philosophers ate enough meals.
 *
 * @return 1 if the must-eat condition is complete, 0 otherwise.
 */
int		enough_meals_eaten(t_dinner *dinner);

/* ************************************************************************** */
/*                                   State                                    */
/* ************************************************************************** */

/**
 * @brief Safely updates the global stop flag.
 */
void	set_stop(t_dinner *dinner, int value);

/**
 * @brief Safely reads the global stop flag.
 */
int		get_stop(t_dinner *dinner);

/* ************************************************************************** */
/*                                    Time                                    */
/* ************************************************************************** */

/**
 * @brief Returns the current timestamp in milliseconds.
 */
long	now_ms(void);

/**
 * @brief Returns milliseconds elapsed since the simulation start.
 */
long	elapsed_ms(t_dinner *dinner);

/**
 * @brief Sleeps in short intervals while checking the stop flag.
 */
void	smart_sleep(t_dinner *dinner, long duration_ms);

/* ************************************************************************** */
/*                                   Output                                   */
/* ************************************************************************** */

/**
 * @brief Prints a synchronized philosopher state message.
 */
void	log_philo(t_philo *philo, const char *msg);

/**
 * @brief Prints an error message and returns the provided value.
 */
int		print_error(const char *msg, int value);

/**
 * @brief Prints the program usage message.
 */
int		print_usage(void);

/* ************************************************************************** */
/*                                   Utils                                    */
/* ************************************************************************** */

int		ft_isdigit(int c);
long	ft_atol_positive(const char *s, int *ok);
int		ft_strcmp(const char *s1, const char *s2);

#endif
