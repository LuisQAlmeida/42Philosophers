#include "philo.h"

static void	*philo_routine(void *arg)
{
	t_thread_arg	*a;

	a = (t_thread_arg *)arg;
	printf("Philosopher thread id %d of %d philosophers.\n",
		a->id, a->rules->n_philo);
	return (NULL);
}

static void	join_created_threads(pthread_t *threads, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
}

static void	free_thread_data(pthread_t *threads, t_thread_arg *args)
{
	free(threads);
	free(args);
}

int	main(int argc, char **argv)
{
	t_rules			rules;
	pthread_t		*threads;
	t_thread_arg	*args;
	int				i;

	if (!parse_rules(&rules, argc, argv))
	{
		printf("Invalid arguments.\n");
		printf("Usage: ./philo n_philo t_die t_eat t_sleep [must_eat]\n");
		return (1);
	}
	threads = malloc(sizeof(pthread_t) * rules.n_philo);
	if (!threads)
		return (printf("malloc failed.\n"), 1);
	args = malloc(sizeof(t_thread_arg) * rules.n_philo);
	if (!args)
		return (free(threads), printf("malloc failed.\n"), 1);
	i = 0;
	while (i < rules.n_philo)
	{
		args[i].id = i + 1;
		args[i].rules = &rules;
		if (pthread_create(&threads[i], NULL, philo_routine, &args[i]) != 0)
		{
			join_created_threads(threads, i);
			free_thread_data(threads, args);
			printf("pthread_create failed.\n");
			return (1);
		}
		i++;
	}
	join_created_threads(threads, rules.n_philo);
	free_thread_data(threads, args);
	return (0);
}
