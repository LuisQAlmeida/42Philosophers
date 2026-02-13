# CodeVault: Short introduction to threads

**Link to video:** https://youtu.be/d9s_d28yJq0?si=1MGAB_JMgZ6ojQJ0

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
