#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <string.h> //memset
# include <stdio.h> //printf
# include <stdlib.h> //malloc, free, exit
# include <unistd.h> //write, usleep, fork
# include <signal.h> //kill
# include <sys/time.h> //gettimeofday
# include <pthread.h> //pthread_create, pthread_detach, pthread_join
# include <sys/wait.h> //waitpid
# include <semaphore.h> /*sem_open, sem_close, sem_post, sem_wait, sem_unlink
			  Might need fcntl.h for sem_open flags like O_CREAT, O_EXCL...
			  and sys/stat.h for sem_open permission bits/macros like S_IRUSR, S_IWUSR...*/

#endif
