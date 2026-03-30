# **Tutorial Notes:** Detached Threads (pthreads) | C Programming Tutorial

## Source
- **ID:** SRC-002
- **Title:** Detached Threads (pthreads) | C Programming Tutorial
- **Creator:** Portfolio Courses
- **Platform:** YouTube
- **Link:** https://www.youtube.com/watch?v=p142bNjIsms
- **Date accessed:** 2026-03-30

## Main concepts covered
- **Difference between joinable and detached threads**  
  The video begins by contrasting **joinable threads** with **detached threads**. A joinable thread is created with `pthread_create`, does some work, and is later synchronized with the main thread using `pthread_join`. A detached thread is also created with `pthread_create`, but then `pthread_detach` is called so the main thread no longer waits for it with `pthread_join`.

  **Joinable thread diagram from the video:**
  ```C
  //      Multi-Threaded Program
  //
  //  |
  //  |   ...                     pthread
  //  |   pthread_create ---------
  //  |   ...                     | ...
  //  |   printf("%d", sum);      | do work
  // Time |   ...                 | ...
  //  |   pthread_join -----------↓
  //  |   ...     |
  //  |   ...     |
  //  ↓       Execution PAUSES here until
  //          thread has done work
  ```

  **Detached thread diagram from the video:**
  ```C
  //      Detached Thread
  //
  //  |
  //  |   ...                     pthread
  //  |   pthread_create ---------
  //  |   pthread_detach          | ...
  //  |   ...                     | ...
  //  |   do work                 | do work
  // Time |   ...                 | ...
  //  |   ...                     | ...
  //  |   ...                     | ...
  //  |   exit                    |-> resources
  //  ↓                           ↓   released
  ```

- **How `pthread_join` affects execution**  
  In the joinable-thread model shown in the video, `pthread_join` pauses execution until the target thread completes its work. If the thread has already finished by the time `pthread_join` runs, then no actual waiting is needed. Otherwise, execution stops there until the thread is done.

  **Core idea from the video:**
  ```C
  pthread_create(&thread, NULL, function, NULL);
  pthread_join(thread, NULL);
  ```

- **How `pthread_detach` changes thread behavior**  
  In the detached-thread model shown in the video, the thread is created normally and then marked as detached with `pthread_detach`. After that, the thread continues running independently until program termination, at which point its resources are released.

  **Core idea from the video:**
  ```C
  pthread_create(&thread, NULL, &logger, NULL);
  pthread_detach(thread);
  ```

- **Detached threads as background processes**  
  The video presents detached threads as a good fit for background work that should happen periodically without blocking the main flow of the program. The chosen example is a background logger that writes the current time and the current state of the program to a log file every second.

- **Using global state shared between the main thread and the detached thread**  
  The logger thread reads shared global variables:
  - `FILE *log_file`
  - `int incidents`

  The main thread updates `incidents` according to user input, while the detached logger thread periodically writes the current value of `incidents` to the log file.

  **Global variables from the video:**
  ```C
  FILE    *log_file;
  int     incidents = 0;
  ```

- **Basic detached-thread logger example**  
  The detached logger thread runs an infinite loop, writes a timestamp and the current number of incidents to the log file, then sleeps for one second before repeating.

  **Logger function from the video:**
  ```C
  void    *logger()
  {
      while (true)
      {
          time_t  current_time;
          time(&current_time);

          struct tm *tm = localtime(&current_time);
          char    timestamp[256];
          strftime(timestamp, 256, "%x@%H:%M:%S", tm);
          fprintf(log_file, "%s: %d\n", timestamp, incidents);
          sleep(1);
      }
      return (NULL);
  }
  ```

- **Using time-related functions to build timestamps**  
  The logger thread builds a timestamp string using functions from `time.h`:
  - `time`
  - `localtime`
  - `strftime`

  The resulting string contains date and time information in the format shown in the video.

  **Timestamp construction from the video:**
  ```C
  time_t  current_time;
  time(&current_time);

  struct tm *tm = localtime(&current_time);
  char    timestamp[256];
  strftime(timestamp, 256, "%x@%H:%M:%S", tm);
  ```

- **Writing log messages with `fprintf`**  
  The video writes each log entry using `fprintf`, combining the generated timestamp with the current value of `incidents`.

  **Logging line from the video:**
  ```C
  fprintf(log_file, "%s: %d\n", timestamp, incidents);
  ```

- **Using `sleep(1)` to make the logger periodic**  
  The detached logger thread sleeps for one second on each loop iteration so that it writes approximately one log entry per second.

  **Loop timing from the video:**
  ```C
  while (true)
  {
      /* build timestamp and write log */
      sleep(1);
  }
  ```

- **Handling errors when opening files and creating/detaching threads**  
  The video checks for errors in:
  - `fopen`
  - `pthread_create`
  - `pthread_detach`

  When an error occurs, the program returns `1` to signal failure.

  **Examples from the video:**
  ```C
  log_file = fopen("log.txt", "w");
  if (log_file == NULL)
  {
      printf("Error opening file.\n");
      return (1);
  }
  ```

  ```C
  int return_value = 0;
  return_value = pthread_create(&thread, NULL, &logger, NULL);
  if (return_value != 0)
      return (1);

  return_value = pthread_detach(thread);
  if (return_value != 0)
      return (1);
  ```

## Key takeaways
- **A joinable thread is synchronized with `pthread_join`.**  
  The main thread can explicitly wait for a joinable thread to finish. This makes thread completion part of the main program flow.

- **A detached thread is not joined back into the main thread.**  
  Once detached, the thread runs independently until the program exits, and its resources are released when the program ends.

- **Detached threads are useful for background tasks.**  
  The video’s logger example shows a natural use case: periodic work that should happen independently while the main thread continues handling user input.

- **The detached thread still starts with `pthread_create`.**  
  Detaching is not a different creation mechanism. The thread is first created normally, then detached using `pthread_detach`.

- **The main thread and detached thread can observe the same program state.**  
  In the example, the detached logger thread periodically records the value of the global `incidents` variable while the main thread updates it.

- **File handling and thread API calls should be checked for errors.**  
  The video consistently checks for failure after `fopen`, `pthread_create`, and `pthread_detach`.

- **Periodic background logging can be implemented with an infinite loop and `sleep(1)`.**  
  The logger function repeatedly:
  1. gets the current time
  2. formats it as a string
  3. writes a log entry
  4. sleeps for one second

## Examples shown in the tutorial
- **Base version of the incident-tracking program**  
  The video starts with a simple program where the user repeatedly enters incident counts until `-1` is entered.

  **Base program from the video:**
  ```C
  #include <stdio.h>

  FILE    *log_file;
  int     incidents = 0;

  int main(void)
  {
      int input = 0;
      do
      {
          printf("Enter -1 to quit.\n");
          printf("New incidents: ");
          scanf("%d", &input);

          if (input != -1)
              incidents += input;
      } while (input != -1);

      return (0);
  }
  ```

- **Creating and detaching the logger thread**  
  The main thread opens the log file, creates the logger thread, checks the return value, then detaches the thread.

  **Thread setup from the video:**
  ```C
  log_file = fopen("log.txt", "w");
  if (log_file == NULL)
  {
      printf("Error opening file.\n");
      return (1);
  }

  pthread_t   thread;
  int         return_value = 0;

  return_value = pthread_create(&thread, NULL, &logger, NULL);
  if (return_value != 0)
      return (1);

  return_value = pthread_detach(thread);
  if (return_value != 0)
      return (1);
  ```

- **Logger function running in the background**  
  The detached thread continuously writes timestamped incident totals to the file.

  **Logger function from the video:**
  ```C
  void    *logger()
  {
      while (true)
      {
          time_t  current_time;
          time(&current_time);

          struct tm *tm = localtime(&current_time);
          char    timestamp[256];
          strftime(timestamp, 256, "%x@%H:%M:%S", tm);
          fprintf(log_file, "%s: %d\n", timestamp, incidents);
          sleep(1);
      }

      return (NULL);
  }
  ```

- **Main thread continuing normal work while logger runs**  
  While the detached logger writes to the file in the background, the main thread continues reading user input and updating the incident count.

  **Main loop from the video:**
  ```C
  int input = 0;
  do
  {
      printf("Enter -1 to quit.\n");
      printf("New incidents: ");
      scanf("%d", &input);

      if (input != -1)
          incidents += input;
  } while (input != -1);
  ```

- **Closing the file before program exit**  
  The video closes the file once the user quits.

  **File close from the video:**
  ```C
  fclose(log_file);
  ```

- **Observed behavior of the generated log**  
  The video demonstrates how `log.txt` reflects the program state over time:
  - repeated `0` entries while no incidents are added
  - `1` entries after the user adds `1`
  - `11` entries after adding `10`
  - quickly changing totals when incidents are entered in rapid succession

## What I can apply immediately
- **Using `pthread_detach` for non-blocking background work**  
  If I have a task that should run in the background without the main thread waiting for it, I can create a thread and detach it.

  **Example from the video:**
  ```C
  pthread_create(&thread, NULL, &logger, NULL);
  pthread_detach(thread);
  ```

- **Building a periodic logger thread**  
  A detached thread can be used to log program state every second while the main thread handles user interaction.

- **Using a loop with `sleep(1)` for repeated background actions**  
  For simple periodic behavior, the pattern shown in the video is straightforward and easy to understand.

  **Example from the video:**
  ```C
  while (true)
  {
      fprintf(log_file, "%s: %d\n", timestamp, incidents);
      sleep(1);
  }
  ```

- **Checking return values from thread-related functions**  
  The video reinforces good practice by verifying whether thread operations succeed.

- **Combining file I/O with a detached thread**  
  The example shows a practical use case where background thread work interacts with file output instead of just printing to the terminal.

- **Understanding one important use case for detached threads**  
  The clearest lesson from the video is that detached threads are useful when the thread should behave like a background helper rather than something the main thread must later rejoin.

## References
- **SRC-002** Detached Threads (pthreads) | C Programming Tutorial
