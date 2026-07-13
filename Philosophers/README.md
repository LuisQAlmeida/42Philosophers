*This project has been created as part of the 42 curriculum by lmanuel-.*

# Philosophers

## Description

Philosophers is a concurrency project based on the classic Dining Philosophers problem.

The program simulates philosophers sitting around a circular table. Each philosopher alternates between eating, sleeping and thinking. To eat, a philosopher must take two forks: the fork on their left and the fork on their right. Since each fork is shared between two neighboring philosophers, access to each fork must be synchronized.

This implementation uses one thread per philosopher and one mutex per fork. Shared simulation state is also protected with dedicated mutexes to avoid data races.

The goal of the project is to correctly handle:

- thread creation and synchronization
- mutex-protected shared resources
- deadlock prevention
- starvation prevention where possible
- accurate death detection
- clean termination
- memory cleanup
- synchronized output

The mandatory version is implemented in the `philo/` directory.

## Instructions

### Compilation

From inside the `philo/` directory:

```bash
make
```

This creates the executable:

```bash
./philo
```

### Cleaning object files

```bash
make clean
```

### Full cleanup

```bash
make fclean
```

### Rebuild

```bash
make re
```

## Usage

```bash
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
```

### Arguments

| Argument | Description |
|---|---|
| `number_of_philosophers` | Number of philosophers and also number of forks. |
| `time_to_die` | Time in milliseconds after which a philosopher dies if they do not start eating. |
| `time_to_eat` | Time in milliseconds a philosopher spends eating. |
| `time_to_sleep` | Time in milliseconds a philosopher spends sleeping. |
| `number_of_times_each_philosopher_must_eat` | Optional. If provided, the simulation stops when every philosopher has eaten at least this many times. |

All arguments must be positive numbers.

## Output Format

Each state change is printed as:

```text
timestamp_in_ms philosopher_id state
```

Possible states:

```text
has taken a fork
is eating
is sleeping
is thinking
died
```

Example:

```text
0 1 has taken a fork
0 1 has taken a fork
0 1 is eating
200 1 is sleeping
311 2 died
```

Output is protected by a mutex to avoid mixed or corrupted lines.

## Implementation Overview

### Threads

Each philosopher is represented by one thread. The main thread creates all philosopher threads, starts the monitor, waits for the simulation to end, then joins all philosopher threads.

### Forks

Each fork is represented by one mutex. A philosopher must lock both fork mutexes before eating and unlock both after eating.

To reduce deadlock risk, philosophers do not all take forks in the same order:

- odd philosophers take the left fork first
- even philosophers take the right fork first

This breaks the circular waiting pattern that could otherwise make all philosophers hold one fork forever.

### Start synchronization

All philosopher threads wait at a start barrier before the simulation officially begins. Once all philosopher threads are ready, the main thread sets the start timestamp and releases them.

This avoids giving an unfair time penalty to philosophers whose threads are created later.

### Shared state

Shared values are protected with mutexes:

| Data | Mutex |
|---|---|
| `stop` flag | `state_mutex` |
| `last_meal_ms` and `meals_eaten` | `meal_mutex` |
| output | `print_mutex` |
| start barrier data | `start_mutex` |
| forks | one mutex per fork |

### Death detection

The main thread monitors all philosophers. A philosopher is considered dead if:

```text
current_time - last_meal_time > time_to_die
```

When a death is detected, the monitor sets the stop flag and prints the death message.

### Meal completion

If the optional `number_of_times_each_philosopher_must_eat` argument is provided, the monitor also checks whether every philosopher has eaten enough times. Once this condition is true, the simulation stops cleanly.

### Timing

The program uses timestamps in milliseconds. Sleeping is implemented with a custom `smart_sleep()` function that sleeps in small intervals and repeatedly checks the stop flag. This allows philosophers to react quickly when the simulation ends.

### Fairness

The implementation uses two small scheduling strategies:

1. Even philosophers wait briefly at the beginning before entering the main loop.
2. For odd numbers of philosophers, a small adaptive thinking delay is used only when there is enough timing margin.

These techniques reduce fork contention and help avoid starvation without hardcoding specific test cases.

## Manual Testing

### Invalid arguments

```bash
./philo
./philo 0 800 200 200
./philo 5 -800 200 200
./philo 5 abc 200 200
./philo 5 800 200 200 0
```

Expected: the program prints usage information and exits.

### One philosopher

```bash
./philo 1 800 200 200
```

Expected: the philosopher takes one fork and dies around `800 ms`.

### Stable simulation

```bash
./philo 5 800 200 200
```

Expected: no philosopher should die.

### Must-eat condition

```bash
./philo 5 800 200 200 7
```

Expected: no philosopher should die, and the simulation should stop once every philosopher has eaten at least 7 times.

### Four philosophers, stable timing

```bash
./philo 4 410 200 200
```

Expected: no philosopher should die.

### Four philosophers, expected death

```bash
./philo 4 310 200 100
```

Expected: one philosopher should die around `310 ms`.

### Memory leaks

```bash
valgrind --leak-check=full --show-leak-kinds=all ./philo 5 310 200 200
```

Expected: no leaks and no memory errors.

### Data races

```bash
valgrind --tool=helgrind ./philo 5 5000 200 200 3
```

Expected: zero reported data race errors.

## Resources

### Official resources

- 42 Philosophers subject
- `pthread_create`
- `pthread_join`
- `pthread_mutex_init`
- `pthread_mutex_lock`
- `pthread_mutex_unlock`
- `pthread_mutex_destroy`
- `gettimeofday`
- `usleep`
- Valgrind Memcheck
- Valgrind Helgrind

### Concepts studied

- threads
- mutexes
- data races
- race conditions
- deadlocks
- starvation
- critical sections
- shared state
- timing precision
- synchronization barriers
- resource cleanup

### AI usage

AI was used as a learning and review assistant to discuss concurrency concepts, testing strategy, project structure, defense preparation, and documentation. The implementation was reviewed, tested, and understood by the student before submission.
