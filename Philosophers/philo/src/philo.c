#include "philo.h"

int	main(int argc, char **argv)
{
	(void)argv;
	if (argc != 5 && argc != 6)
	{
		printf("You should insert either 4 or 5 arguments. No more, no less...\n");
		return (1);
	}
	printf("You've inserted the right amount of arguments!\n");
	return (0);
}
