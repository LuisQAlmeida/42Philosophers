/* CodeVault: Short introduction to threads
	Link: https://youtu.be/d9s_d28yJq0?si=1MGAB_JMgZ6ojQJ0
This tutorial shows how to create threads.
- First we need to declare a structure of type pthread_t
- Then we call the function pthread_create to create the thread with 
the following parameters:
	- 1st parameter: Pointer to the thread that's being created;
	- 2nd parameter: Attributes (not explored in this tutorial);
	- 3rd parameter: Pointer to the function the the thread is 
	going to run;
	- 4th parameter: Don't remember, need to watch video again.
- After creating the thread with pthread_create, we have to call 
pthread_join, which is sort of like the wait function for threads. 
It allows the thread to run correctly in its entirety. This function 
takes the following parameters:
	- 1st parameter: the thread variable;
	- Don't remember, need to watch video again.
- Both pthread_create & pthread_join return 0 on success.
- At compilation, we need to use the -pthread flag to be able to 
link pthread library correctly.
*/

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
