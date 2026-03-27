# **Tutorial Notes:** Introduction to Threads (pthreads) | C Programming Tutorial

## Source
- **ID:** SRC-001
- **Title:** Introduction To Threads (pthreads) | C Programming Tutorial
- **Creator:** Portfolio Courses
- **Platform:** YouTube
- **Link:** https://www.youtube.com/watch?v=ldJ8WGZVXZk
- **Date accessed:** 2026-03-27

## Main concepts covered
- **Difference between single-threaded and multi-threaded execution**  
  A regular C program is described as a **single-threaded program**, meaning it has one sequence of statements executing over time. A **multi-threaded program** has multiple sequences of statements executing over the same period of time.  
  In the video, this is first illustrated conceptually with one sequence of operations for a single-threaded program, and then with two separate sequences for a multi-threaded one.

  **Single-threaded example from the video:**
  ```C
  //              |   int x;
  //              |   x = 20;
  //              |   int y;
  //    Time      |   y = 50;
  //              |   int sum;
  //              |   sum = x + y;
  //              ↓
  ```

  **Multi-threaded example from the video:**
  ```C
  //              |   int x;               |   int a;
  //              |   x = 20;              |   a = 3;
  //              |   int y;               |   int b;
  //    Time      |   y = 50;              |   b = 5;
  //              |   int sum;             |   int product;
  //              |   sum = x + y;         |   product = a * b;
  //              ↓                        ↓
  //
  //              Parallel Execution
  ```

- **Concurrent execution vs parallel execution**  
  The video distinguishes between **parallel** and **concurrent** execution. If two threads execute statements at the exact same moment, they are executing in **parallel**. If the CPU can only execute one thread at a time, then it switches back and forth between threads, which is **concurrent execution**.  
  So even though both threads may make progress during the same time period, only one may actually be executing at any instant.

  **Concurrent but not parallel example from the video:**
  ```C
  //              |   int x;               |
  //              |                        |   int a;
  //              |   x = 20;              |
  //              |   int y;               |
  //    Time      |                        |   int b;
  //              |   y = 50;              |
  //              |                        |   b = 5;
  //              |                        |   int product;
  //              |   int sum;             |
  //              |   sum = x + y;         |
  //              |                        |   product = a * b;
  //              ↓                        ↓
  //
  //              Concurrent But Not Parallel Execution
  ```

  The main idea presented in the video is that threads may execute either concurrently or in parallel depending on the system.

- **Basic introduction to POSIX threads (`pthread`)**  
  To create threads in C, the video uses the **pthread library**, also called the **POSIX thread library**. The general idea presented is that the main thread can call `pthread_create` to start another thread running a specific function, and later use `pthread_join` to wait for that thread to finish.  

  **Conceptual example from the video:**
  ```C
  //              |               pthread
  //              |
  //              |   sum = x + y;
  //              |   pthread_create ---------- function()
  //              |   ...                       | int a = 5;
  //    Time      |   printf("%d", sum);        | int b = 3;
  //              |   ...                       | int result = x + y;
  //              |   pthread_join ------------ ↓
  //              |   ...
  //              |   ...
  //              ↓          execution PAUSES here until thread done
  //
  //              Parallel Execution
  ```

- **Creating threads with `pthread_create`**  
  The video explains that `pthread_create` is used to start a new thread that runs a given function.  
  In the first practical example, a `pthread_t` variable is declared and then passed by address to `pthread_create`.

  **Example from the video:**
  ```C
  pthread_t   thread1;

  pthread_create(&thread1, NULL, computation, NULL);
  ```

  **Arguments explained in the video:**
  - `&thread1` stores the thread identifier
  - `NULL` means default thread attributes are used
  - `computation` is the function the new thread will run
  - `NULL` means no argument is being passed to the function in this first example

- **Waiting for threads with `pthread_join`**  
  The video explains that `pthread_join` is used to join the created thread back with the main thread. If the created thread has not finished by the time `pthread_join` is reached, execution pauses until that thread completes its work.  
  If the thread has already finished, `pthread_join` simply continues.

  **Example from the video:**
  ```C
  pthread_join(thread1, NULL);
  ```

  **Arguments explained in the video:**
  - `thread1` is the thread to wait for
  - `NULL` means the return value of the thread function is not being used

- **Passing arguments to thread routines through `void *`**  
  The video shows that thread functions can receive arguments, but the argument is expected to be a `void *`.  
  This allows passing the address of a value, or even the address of a struct, into the thread function. Inside the function, the pointer must be cast back to the appropriate type.

  **Example from the video:**
  ```C
  long    value1 = 1;

  pthread_create(&thread1, NULL, computation, (void *)&value1);
  ```

  **Inside the thread routine:**
  ```C
  void    *computation(void *add)
  {
      long    *add_num = (long *)(add);
      printf("Add: %ld\n", *add_num);
      return (NULL);
  }
  ```

  The video explains this as a roundabout but useful technique: pass the address as a `void *`, then cast it back and dereference it inside the function.

- **Basic thread routine structure in C**  
  The thread function used with `pthread_create` must return a `void *`. In the first example, the function takes no explicit argument, and in the second version it is changed to accept a `void *` parameter so that data can be passed to it.

  **First example from the video:**
  ```C
  void    *computation()
  {
      printf("Computation\n");
      return (NULL);
  }
  ```

  **Argument-passing version from the video:**
  ```C
  void    *computation(void *add)
  {
      long    *add_num = (long *)(add);
      printf("Add: %ld\n", *add_num);
      return (NULL);
  }
  ```

## Key takeaways
- **A thread is an independent execution path inside a process.**  
  The video presents a thread as its own sequence of statements executing over time. A single-threaded program has one sequence, while a multi-threaded program has multiple sequences.

- **Multiple threads in the same process share the same address space and resources.**  
  While the video does not go deeply into memory-sharing details, it demonstrates multiple threads running within the same C program and being managed from the same `main` function.

- **`pthread_t` is used to identify and manage threads.**  
  In each example, a `pthread_t` variable is created first, then passed to `pthread_create`, and later used with `pthread_join`.

  **Example from the video:**
  ```C
  pthread_t   thread1;
  pthread_t   thread2;
  ```

- **`pthread_create` starts a new thread and requires a function with the signature `void *routine(void *arg)`.**  
  The video shows that the thread function must return a `void *`, and when arguments are passed, the function can receive a `void *` parameter.

- **`pthread_join` blocks until the target thread finishes execution.**  
  This is emphasized clearly in the video. It is what causes execution to pause until the thread has completed its work.

- **Thread arguments are passed as `void *`, so careful casting is required inside the thread routine.**  
  The video demonstrates passing the address of `value1` as a `void *`, then casting it to `long *` inside the thread function and dereferencing it to print the value.

- **Multi-threaded execution is not automatically the same as true parallel execution. On a single core, execution may still be concurrent without being parallel.**  
  This is one of the main conceptual distinctions made in the video. The CPU may switch between threads, allowing both to progress over time, even if only one is executing at each instant.

## Examples shown in the tutorial
- **Creating two threads from `main`**  
  The video extends the basic example by creating a second thread and a second value, then launching both threads.

  **Example from the video:**
  ```C
  pthread_t   thread1;
  pthread_t   thread2;
  long        value1 = 1;
  long        value2 = 2;

  pthread_create(&thread1, NULL, computation, (void *)&value1);
  pthread_create(&thread2, NULL, computation, (void *)&value2);
  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);
  ```

- **Passing different values to each thread through pointers**  
  Each thread is given the address of a different variable, so when the thread function runs, it prints a different value.

  **Example output from the video:**
  ```text
  Add: 1
  Add: 2
  ```

- **Casting the `void *` argument back to the expected type inside the thread routine**  
  The `void *` argument is converted back into a `long *`, allowing access to the original value.

  **Example from the video:**
  ```C
  long    *add_num = (long *)(add);
  printf("Add: %ld\n", *add_num);
  ```

- **Using `pthread_join` to make sure the main thread waits for worker threads before exiting**  
  The examples consistently call `pthread_join` after creating the threads so that the main thread waits for them to complete.

  **Example from the video:**
  ```C
  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);
  ```

## What I can apply immediately
- **Basic `pthread` setup**  
  Include the pthread header and use `pthread_t`, `pthread_create`, and `pthread_join`.

  **Example from the video:**
  ```C
  #include <pthread.h>
  ```

- **Thread creation loop**  
  The video does not use a loop, but the same pattern shown can be repeated for multiple threads.

- **Thread join loop**  
  The video joins each thread explicitly one by one. The same idea could later be adapted into a loop for larger numbers of threads.

- **Passing per-thread data through structs**  
  The video mentions that besides passing a value through a pointer, it is also possible to pass something like a struct through a pointer.

- **Understanding that the thread routine must match the expected function signature**  
  One immediate practical lesson from the video is that the thread function must be designed in the correct form for use with `pthread_create`, especially when arguments need to be passed.

- **Understanding the performance effect of thread placement and joining order**  
  The video shows that using threads can improve performance, but only if the threads are allowed to execute during the same period of time.

  **Single-threaded performance test from the video:**
  ```C
  computation((void *)&value1);
  computation((void *)&value2);
  ```
  This took around **4.5 to 5.0 seconds**.

  **Multi-threaded performance test from the video:**
  ```C
  pthread_create(&thread1, NULL, computation, (void *)&value1);
  pthread_create(&thread2, NULL, computation, (void *)&value2);
  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);
  ```
  This took around **2.5 seconds**.

  **Sequential joining example from the video:**
  ```C
  pthread_create(&thread1, NULL, computation, (void *)&value1);
  pthread_join(thread1, NULL);
  pthread_create(&thread2, NULL, computation, (void *)&value2);
  pthread_join(thread2, NULL);
  ```
  This returned to around **4.5 seconds**, because the second thread only starts after the first one has already finished, making it effectively similar to single-threaded execution.

## References
- **SRC-001** Introduction To Threads (pthreads) | C Programming Tutorial
