#ifndef PHILO_H
# define PHILO_H

# include <limits.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

/**
 * @file philo.h
 * @brief Shared data model and maintained interface for Philosophers.
 *
 * The maintained implementation models each philosopher as one POSIX worker
 * thread and each fork as one mutex.
 *
 * Synchronization is split by responsibility instead of placing the entire
 * simulation behind one global lock:
 *
 * - each fork mutex represents exclusive ownership of one fork;
 * - start_mutex protects the coordinated-start state;
 * - meal_mutex protects per-philosopher meal state and the starvation
 *   read-and-decide interval;
 * - state_mutex protects the global stop flag;
 * - print_mutex serializes event output.
 *
 * Workers reach a mutex-protected polling start gate before normal execution.
 * The main thread establishes the common start timestamp and initial meal
 * timestamps before opening that gate.
 *
 * Relevant nested locking relationships include:
 *
 * @code
 * meal_mutex
 *     -> state_mutex
 *
 * print_mutex
 *     -> state_mutex
 * @endcode
 *
 * A worker may also hold one or both fork mutexes while updating meal state or
 * producing an event.
 *
 * Start initialization uses meal_mutex and start_mutex sequentially rather
 * than holding both simultaneously.
 *
 * These mechanisms coordinate the maintained implementation but are not
 * claims of formal scheduler fairness or starvation freedom for every possible
 * thread interleaving.
 */

/* ************************************************************************** */
/*                                Structures                                  */
/* ************************************************************************** */

/**
 * @brief Forward declaration of the shared simulation context.
 */
typedef struct s_dinner	t_dinner;

/**
 * @brief Immutable runtime configuration for one simulation.
 *
 * Values are populated during command-line parsing before worker threads are
 * created and are treated as read-only configuration during the simulation.
 */
typedef struct s_rules
{
	/** @brief Number of philosophers and therefore number of forks. */
	int		n_philo;

	/** @brief Maximum interval without a new meal start, in milliseconds. */
	long	t_die;

	/** @brief Duration of the eating action, in milliseconds. */
	long	t_eat;

	/** @brief Duration of the sleeping action, in milliseconds. */
	long	t_sleep;

	/**
	 * @brief Required meal count when the optional completion target is active.
	 */
	int		must_eat;

	/**
	 * @brief Indicates whether the optional meal-count argument was supplied.
	 */
	int		has_must_eat;
}	t_rules;

/**
 * @brief State belonging to one philosopher worker.
 *
 * Identity, fork indexes, the shared-context pointer, and initial meal state
 * are assigned before the worker begins its normal simulation lifecycle.
 *
 * During concurrent execution, meals_eaten and last_meal_ms are accessed
 * through the shared dinner meal_mutex.
 */
typedef struct s_philo
{
	/** @brief One-based philosopher identifier used in event output. */
	int			id;

	/** @brief Index of this philosopher's left fork mutex. */
	int			left_fork;

	/** @brief Index of this philosopher's right fork mutex. */
	int			right_fork;

	/**
	 * @brief Number of registered meal starts.
	 *
	 * Concurrent reads and writes are serialized through meal_mutex.
	 */
	int			meals_eaten;

	/**
	 * @brief Timestamp of the most recent meal start, in milliseconds.
	 *
	 * Concurrent reads and writes, including the starvation decision, are
	 * serialized through meal_mutex.
	 */
	long		last_meal_ms;

	/**
	 * @brief POSIX worker-thread handle.
	 *
	 * The main simulation flow creates the thread and later joins it.
	 */
	pthread_t	thread;

	/** @brief Back-reference to the shared simulation context. */
	t_dinner	*dinner;
}	t_philo;

/**
 * @brief Shared context and synchronization state for one simulation.
 *
 * The main thread owns initialization, worker creation, monitoring, joining,
 * and final cleanup of this context.
 *
 * The dynamically allocated forks and philos arrays are owned by the
 * simulation lifecycle and released during cleanup.
 */
typedef struct s_dinner
{
	/** @brief Parsed configuration, treated as immutable after initialization. */
	t_rules			rules;

	/**
	 * @brief Common simulation start timestamp in milliseconds.
	 *
	 * release_start() establishes this value before opening the start gate.
	 * It is then treated as read-only timing state.
	 */
	long			start_ms;

	/**
	 * @brief Coordinated-start gate flag protected by start_mutex.
	 */
	int				start;

	/**
	 * @brief Global termination flag protected by state_mutex.
	 */
	int				stop;

	/**
	 * @brief Number of worker threads that reached the start gate.
	 *
	 * Reads and writes are protected by start_mutex.
	 */
	int				ready_count;

	/**
	 * @brief Serializes event-output decisions and complete output lines.
	 *
	 * log_philo() holds this mutex while checking global stop state and
	 * printing an event.
	 */
	pthread_mutex_t	print_mutex;

	/**
	 * @brief Protects the global stop flag.
	 */
	pthread_mutex_t	state_mutex;

	/**
	 * @brief Protects philosopher meal state and starvation decisions.
	 *
	 * The protected state includes meals_eaten and last_meal_ms. The maintained
	 * starvation check keeps the timestamp read and death decision within this
	 * lock so that a new meal cannot be registered between those operations.
	 *
	 * release_start() also uses this mutex while establishing start_ms and the
	 * initial last_meal_ms value of every philosopher before the start gate is
	 * opened.
	 */
	pthread_mutex_t	meal_mutex;

	/**
	 * @brief Protects ready_count and the coordinated-start gate flag.
	 */
	pthread_mutex_t	start_mutex;

	/**
	 * @brief Dynamically allocated array containing one mutex per fork.
	 *
	 * During the simulation, locking forks[i] represents exclusive ownership of
	 * physical fork i.
	 */
	pthread_mutex_t	*forks;

	/**
	 * @brief Dynamically allocated array containing all philosopher states.
	 *
	 * Worker threads receive pointers to elements of this array. The main
	 * simulation flow joins the workers before freeing it.
	 */
	t_philo			*philos;
}	t_dinner;

/* ************************************************************************** */
/*                                  Parsing                                   */
/* ************************************************************************** */

/**
 * @brief Parses and validates the command-line simulation configuration.
 *
 * @param rules Destination configuration structure.
 * @param ac Argument count.
 * @param av Argument vector.
 * @return Non-zero on success, zero when the arguments are invalid.
 */
int		parse_rules(t_rules *rules, int ac, char **av);

/* ************************************************************************** */
/*                                Simulation                                  */
/* ************************************************************************** */

/**
 * @brief Registers a worker at the coordinated start gate and waits for release.
 *
 * The worker contributes to ready_count under start_mutex and waits until the
 * protected start flag is set or global termination is requested.
 *
 * @param philo Philosopher entering the start gate.
 */
void	wait_for_start(t_philo *philo);

/**
 * @brief Establishes the common timing baseline and opens the start gate.
 *
 * The function establishes start_ms and every philosopher's initial
 * last_meal_ms while holding meal_mutex. It then releases meal_mutex and opens
 * the gate by setting start under start_mutex.
 *
 * @param dinner Shared simulation context.
 */
void	release_start(t_dinner *dinner);

/**
 * @brief Tests whether every worker has reached the coordinated start gate.
 *
 * ready_count is read while holding start_mutex.
 *
 * @param dinner Shared simulation context.
 * @return Non-zero when every configured philosopher is ready, otherwise zero.
 */
int		all_philos_ready(t_dinner *dinner);

/**
 * @brief Allocates philosopher state, runs the simulation, and joins workers.
 *
 * @param dinner Initialized shared simulation context.
 * @return Non-zero on success, zero if simulation startup fails.
 */
int		start_simulation(t_dinner *dinner);

/* ************************************************************************** */
/*                                  Actions                                   */
/* ************************************************************************** */

/**
 * @brief Executes one philosopher eating action.
 *
 * The philosopher acquires both assigned fork mutexes before registering the
 * meal state and waiting for the configured eating duration.
 *
 * @param philo Philosopher performing the action.
 */
void	philo_eat(t_philo *philo);

/**
 * @brief Logs and performs the configured sleeping action.
 *
 * @param philo Philosopher performing the action.
 */
void	philo_sleep(t_philo *philo);

/**
 * @brief Logs the thinking state and applies any contention-management delay.
 *
 * @param philo Philosopher performing the action.
 */
void	philo_think(t_philo *philo);

/**
 * @brief Handles the one-philosopher case where only one fork exists.
 *
 * @param philo Sole philosopher in the simulation.
 */
void	handle_single_philo(t_philo *philo);

/* ************************************************************************** */
/*                                   Forks                                    */
/* ************************************************************************** */

/**
 * @brief Allocates and initializes one fork mutex per philosopher.
 *
 * @param dinner Shared simulation context.
 * @return Non-zero on success, zero if allocation or mutex initialization
 * fails.
 */
int		init_forks(t_dinner *dinner);

/**
 * @brief Destroys initialized fork mutexes and releases the fork array.
 *
 * @param dinner Shared simulation context.
 * @param count Number of initialized fork mutexes to destroy.
 */
void	destroy_forks(t_dinner *dinner, int count);

/**
 * @brief Acquires both fork mutexes assigned to a philosopher.
 *
 * Acquisition order depends on philosopher identity to avoid a uniform
 * circular acquisition pattern. This is a deadlock-avoidance strategy, not a
 * formal fairness guarantee.
 *
 * @param philo Philosopher acquiring its forks.
 */
void	take_forks(t_philo *philo);

/**
 * @brief Releases both fork mutexes owned by a philosopher.
 *
 * @param philo Philosopher releasing its forks.
 */
void	drop_forks(t_philo *philo);

/* ************************************************************************** */
/*                                Meal State                                  */
/* ************************************************************************** */

/**
 * @brief Records the start of a meal.
 *
 * last_meal_ms and meals_eaten are updated together while meal_mutex is held.
 *
 * @param philo Philosopher beginning a meal.
 */
void	update_meal_state(t_philo *philo);

/**
 * @brief Atomically evaluates the maintained starvation decision.
 *
 * The function keeps meal_mutex locked while reading last_meal_ms and deciding
 * whether the starvation deadline has been reached. If death is detected it
 * publishes global termination through set_stop(), which acquires state_mutex.
 *
 * @param philo Philosopher to evaluate.
 * @return Non-zero when starvation is detected, otherwise zero.
 */
int		stop_if_starved(t_philo *philo);

/**
 * @brief Reads a philosopher's meal count under meal_mutex.
 *
 * @param philo Philosopher whose count is requested.
 * @return Number of registered meal starts.
 */
int		get_meals_eaten(t_philo *philo);

/* ************************************************************************** */
/*                                  Monitor                                   */
/* ************************************************************************** */

/**
 * @brief Runs the main-thread monitoring loop until simulation termination.
 *
 * The monitor checks starvation and, when configured, the per-philosopher meal
 * completion target.
 *
 * @param dinner Shared simulation context.
 */
void	monitor_dinner(t_dinner *dinner);

/**
 * @brief Evaluates whether one philosopher has reached the starvation deadline.
 *
 * @param philo Philosopher to inspect.
 * @return Non-zero when the philosopher is dead, otherwise zero.
 */
int		is_philo_dead(t_philo *philo);

/**
 * @brief Tests whether all philosophers reached the optional meal target.
 *
 * @param dinner Shared simulation context.
 * @return Non-zero when the configured target is active and every philosopher
 * has reached it, otherwise zero.
 */
int		enough_meals_eaten(t_dinner *dinner);

/* ************************************************************************** */
/*                                   State                                    */
/* ************************************************************************** */

/**
 * @brief Updates the global termination flag under state_mutex.
 *
 * @param dinner Shared simulation context.
 * @param value New stop value.
 */
void	set_stop(t_dinner *dinner, int value);

/**
 * @brief Reads the global termination flag under state_mutex.
 *
 * @param dinner Shared simulation context.
 * @return Current stop value.
 */
int		get_stop(t_dinner *dinner);

/* ************************************************************************** */
/*                                    Time                                    */
/* ************************************************************************** */

/**
 * @brief Returns the current wall-clock timestamp in milliseconds.
 *
 * @return Current timestamp in milliseconds.
 */
long	now_ms(void);

/**
 * @brief Returns milliseconds elapsed since the common simulation start.
 *
 * @param dinner Shared simulation context containing start_ms.
 * @return Elapsed milliseconds.
 */
long	elapsed_ms(t_dinner *dinner);

/**
 * @brief Waits approximately the requested duration while remaining stop-aware.
 *
 * The implementation repeatedly checks global termination instead of issuing
 * one uninterruptible sleep for the entire duration.
 *
 * @param dinner Shared simulation context.
 * @param duration_ms Requested duration in milliseconds.
 */
void	smart_sleep(t_dinner *dinner, long duration_ms);

/* ************************************************************************** */
/*                                   Output                                   */
/* ************************************************************************** */

/**
 * @brief Emits one serialized philosopher event when output is still valid.
 *
 * print_mutex serializes the output decision and complete line. The function
 * reads global stop state through get_stop(), producing the maintained
 * print_mutex -> state_mutex nesting relationship. A death event remains
 * eligible for output after the global stop flag is set.
 *
 * @param philo Philosopher associated with the event.
 * @param msg Event message.
 */
void	log_philo(t_philo *philo, const char *msg);

/**
 * @brief Writes an error message and returns a caller-selected value.
 *
 * @param msg Error message.
 * @param value Value returned to the caller.
 * @return value.
 */
int		print_error(const char *msg, int value);

/**
 * @brief Prints command-line usage information.
 *
 * @return Failure-style value used by the top-level argument path.
 */
int		print_usage(void);

/* ************************************************************************** */
/*                                   Utils                                    */
/* ************************************************************************** */

/**
 * @brief Tests whether a character value represents a decimal digit.
 *
 * @param c Character value.
 * @return Non-zero for a decimal digit, otherwise zero.
 */
int		ft_isdigit(int c);

/**
 * @brief Parses a positive long value with validation.
 *
 * @param s String to parse.
 * @param ok Output flag indicating whether parsing succeeded.
 * @return Parsed value when valid.
 */
long	ft_atol_positive(const char *s, int *ok);

/**
 * @brief Compares two null-terminated strings.
 *
 * @param s1 First string.
 * @param s2 Second string.
 * @return Zero when equal, otherwise the first differing byte comparison.
 */
int		ft_strcmp(const char *s1, const char *s2);

#endif
