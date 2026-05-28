#include "philo.h"

static void	*philo_routine(void *arg)
{
	t_thread_arg	*a;

	a = (t_thread_arg *)arg;
	if (!get_stop(a->dinner))
		log_philo(a, "started");
	return (NULL);
}

static void	join_philos(pthread_t *threads, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
}

static void	free_philo_data(pthread_t *threads, t_thread_arg *args)
{
	free(threads);
	free(args);
}

static int	create_philos(t_dinner *dinner,
		pthread_t *threads, t_thread_arg *args)
{
	int	i;

	i = 0;
	while (i < dinner->rules.n_philo)
	{
		args[i].id = i + 1;
		args[i].dinner = dinner;
		if (pthread_create(&threads[i], NULL, philo_routine, &args[i]) != 0)
		{
			join_philos(threads, i);
			return (0);
		}
		i++;
	}
	return (1);
}

int	start_thread_test(t_dinner *dinner)
{
	pthread_t		*threads;
	t_thread_arg	*args;

	threads = malloc(sizeof(pthread_t) * dinner->rules.n_philo);
	if (!threads)
		return (printf("malloc failed.\n"), 0);
	args = malloc(sizeof(t_thread_arg) * dinner->rules.n_philo);
	if (!args)
		return (free(threads), printf("malloc failed.\n"), 0);
	if (!create_philos(dinner, threads, args))
	{
		free_philo_data(threads, args);
		return (printf("pthread_create failed.\n"), 0);
	}
	join_philos(threads, dinner->rules.n_philo);
	free_philo_data(threads, args);
	return (1);
}
