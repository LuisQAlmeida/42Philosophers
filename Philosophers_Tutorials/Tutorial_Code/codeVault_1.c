#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void	*routine()
{
	printf("ROUTINE START....\n");
	sleep(3);
	printf("ENDING ROUTINE.\n");
	return (NULL);
}

int	main(void)
{
	pthread_t	thread_1;
	pthread_t	thread_2;

	printf("[PROGRAM START]\n");
	sleep(1);
	if (pthread_create(&thread_1, NULL, &routine, NULL) != 0)
		return (1);
	if (pthread_create(&thread_2, NULL, &routine, NULL) != 0)
		return (2);
	if (pthread_join(thread_1, NULL) != 0)
		return (3);
	if (pthread_join(thread_2, NULL) != 0)
		return (4);
	sleep(1);
	printf("[PROGRAM END]\n");
	sleep(2);
	printf("\nPROGRAM NOTES:\n");
	printf("From this tutorial, we can observe that both threads\n");
	printf("are running in parallel, since they both print 'ROUTINE START....'\n");
	printf("and only after this do we see 'ENDING ROUTINE.' being printed\n");
	printf("eventhough both threads are calling the same routine function\n");
	printf("which prints 'ROUTINE START....' followed by 'ENDING ROUTINE.'.\n");
	return (0);
}
