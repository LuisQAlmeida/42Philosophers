# Tutorial Notes: Introduction to Threads (pthreads)

## Source
- **ID:** SRC-001
- **Title:** Introduction To Threads (pthreads) | C Programming Tutorial
- **Creator:** Portfolio Courses
- **Platform:** YouTube
- **Link:** https://www.youtube.com/watch?v=ldJ8WGZVXZk
- **Date accessed:** 2026-03-23

## Why I watched this
I watched this tutorial to build a solid foundation for the `philosophers` project, especially to understand:
- what a thread is
- how multiple threads run inside one process
- how `pthread_create` and `pthread_join` work
- how thread functions receive arguments

## Main concepts covered
- Difference between single-threaded and multi-threaded execution
- Concurrent execution vs parallel execution
- Basic introduction to POSIX threads (`pthread`)
- Creating threads with `pthread_create`
- Waiting for threads with `pthread_join`
- Passing arguments to thread routines through `void *`
- Basic thread routine structure in C

## Key takeaways
- A thread is an independent execution path inside a process.
- Multiple threads in the same process share the same address space and resources.
- `pthread_t` is used to identify and manage threads.
- `pthread_create` starts a new thread and requires a function with the signature `void *routine(void *arg)`.
- `pthread_join` blocks until the target thread finishes execution.
- Thread arguments are passed as `void *`, so careful casting is required inside the thread routine.
- Multi-threaded execution is not automatically the same as true parallel execution. On a single core, execution may still be concurrent without being parallel.  

## Useful examples from the tutorial
- Creating two threads from `main`
- Passing different values to each thread through pointers
- Casting the `void *` argument back to the expected type inside the thread routine
- Using `pthread_join` to make sure the main thread waits for worker threads before exiting

## Relevance to Philosophers
This tutorial is directly relevant to `philosophers` because:
- each philosopher is typically represented by a thread
- the program needs to create multiple threads and manage their lifecycle
- the project depends on understanding what each thread is doing independently
- thread creation and joining are core to setting up and shutting down the simulation cleanly

## What I learned that matters for my implementation
- I need a thread routine per philosopher, usually something like `void *philo_routine(void *arg)`.
- I will pass each philosopher structure as the thread argument.
- I must cast the generic `void *` argument back to `t_philo *` inside the routine.
- The main program must keep track of all created threads.
- I will likely need to join philosopher threads at the end so the program exits cleanly.

## Important limitations of this tutorial for Philosophers
This tutorial is a good introduction, but it does **not** fully cover the harder parts of `philosophers`, such as:
- mutex protection of shared data
- race conditions
- deadlocks
- starvation
- safe reading/writing of shared timestamps and stop flags

So this source is best treated as a **threads introduction**, not as a full concurrency guide.

## What I can apply immediately
- Basic `pthread` setup
- Thread creation loop
- Thread join loop
- Passing per-thread data through structs
- Understanding that the thread routine must match the expected function signature

## What I still need to study after this
- `pthread_mutex_init`, `pthread_mutex_lock`, `pthread_mutex_unlock`, `pthread_mutex_destroy`
- protecting shared state
- deadlock avoidance strategies
- monitor thread or death-check loop design
- precise timing and synchronized printing

## References
- **SRC-001** Introduction To Threads (pthreads) | C Programming Tutorial
