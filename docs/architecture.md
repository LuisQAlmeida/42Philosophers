# Architecture

## Purpose

The mandatory Philosophers program models the Dining Philosophers problem using
one POSIX thread per philosopher and one mutex per fork.

The main thread owns initialization, coordinated startup, monitoring, joining
and final cleanup. Philosopher threads perform the simulated
eat-sleep-think lifecycle.

## Core data model

Three structures represent the simulation.

### `t_rules`

`t_rules` stores immutable runtime configuration:

| Field | Meaning |
|---|---|
| `n_philo` | number of philosophers and forks |
| `t_die` | starvation deadline in milliseconds |
| `t_eat` | eating duration |
| `t_sleep` | sleeping duration |
| `must_eat` | optional meal target |
| `has_must_eat` | whether the optional target was supplied |

### `t_philo`

Each philosopher stores:

| Field | Meaning |
|---|---|
| `id` | one-based philosopher identifier |
| `left_fork` | left fork index |
| `right_fork` | right fork index |
| `meals_eaten` | number of registered meal starts |
| `last_meal_ms` | timestamp of the latest meal start |
| `thread` | POSIX thread handle |
| `dinner` | pointer to shared simulation state |

Fork indexes are assigned circularly:

    left_fork  = i
    right_fork = (i + 1) % n_philo

### `t_dinner`

`t_dinner` is the shared simulation context.

It owns:

- the parsed rules;
- the common start timestamp;
- readiness and stop state;
- shared synchronization mutexes;
- the dynamically allocated fork array;
- the dynamically allocated philosopher array.

## Initialization flow

Execution begins in `main()`.

    main
      |
      +-- zero t_dinner
      +-- parse_rules()
      `-- start_dinner()
            |
            +-- initialize shared mutexes
            +-- initialize fork mutexes
            `-- start_simulation()

Shared mutex initialization is transactional. If one mutex fails to initialize,
the mutexes already created are destroyed before returning an error.

Fork initialization follows the same principle: if initialization fails partway
through the fork array, the successfully created mutexes are destroyed and the
allocation is released.

## Philosopher allocation

`start_simulation()` allocates one `t_philo` object per configured philosopher.

The byte count is calculated as a `size_t`:

    sizeof(t_philo) * (size_t)n_philo

The same byte count is used for allocation and zero-initialization.

`init_philos()` then assigns IDs, fork indexes, initial meal state and the shared
`t_dinner` pointer.

## Thread creation

`create_philos()` launches each worker with `pthread_create()`.

Every worker executes the private `philo_routine()` function.

If thread creation fails:

    set global stop
    join workers already created
    free philosopher storage
    return failure

This prevents successfully started workers from being abandoned on a partial
startup failure.

## Coordinated start

A newly created philosopher does not immediately begin competing for forks.

Each worker first calls `wait_for_start()`:

    worker
      |
      +-- increment ready_count under start_mutex
      `-- poll protected start flag

The main thread waits until:

    ready_count == n_philo

It then calls `release_start()`.

Under `meal_mutex`, the function:

    start_ms = now_ms()
    each last_meal_ms = start_ms

Only after all initial meal timestamps are established does it set `start = 1`
under `start_mutex`.

This avoids charging later-created threads for time spent creating the rest of
the worker set.

The mechanism is a mutex-protected polling gate, not a `pthread_barrier_t` or
condition-variable implementation.

## Worker lifecycle

After the start gate opens, the worker checks whether the simulation has
already stopped.

The one-philosopher case has its own path because a single philosopher can
acquire only one fork.

For simulations with multiple philosophers, even IDs initially sleep for half
of `t_eat` to stagger contention.

The normal worker loop is:

    while simulation is active
        eat
        sleep
        think

### Eat

`philo_eat()`:

    check stop
    acquire both forks
    check stop again
    update meal timestamp and meal count
    log "is eating"
    smart_sleep(t_eat)
    release both forks

The second stop check is important because a worker may have spent time blocked
inside fork acquisition.

### Sleep

`philo_sleep()` logs the state and waits for `t_sleep` while remaining
responsive to global termination.

### Think

`philo_think()` logs the state and optionally applies a calculated contention
delay.

The extra delay is used only for odd philosopher counts and only when the
configured timing relationships make the delay meaningful.

## Single-philosopher path

With one philosopher there is only one fork.

The worker therefore:

    lock its only fork
    log "has taken a fork"
    wait for t_die or global stop
    unlock the fork

The monitor independently reaches the starvation deadline and terminates the
simulation.

## Monitoring

After releasing the start gate, the main thread becomes the monitor by calling
`monitor_dinner()`.

The monitor is not a separate pthread.

Its loop performs:

    check every philosopher for starvation
    check optional meal-completion condition
    sleep briefly
    repeat

Starvation is checked through `stop_if_starved()`.

When the optional meal target is active, the monitor also reads each
`meals_eaten` counter and stops once every philosopher has reached the target.

## Joining and cleanup

After the monitor returns, `start_simulation()` joins every philosopher thread.

It then frees the philosopher array.

Control returns to `start_dinner()`, which:

    destroys every fork mutex
    frees the fork array
    destroys shared mutexes

Successful execution therefore leaves no simulation allocation owned by the
program.

## Source layout

| File | Responsibility |
|---|---|
| `main.c` | top-level initialization and shared-mutex lifecycle |
| `parse.c` | command-line rules |
| `simulation.c` | philosopher allocation, thread creation and lifecycle |
| `start_sync.c` | coordinated startup |
| `actions.c` | eat, sleep, think and single-philosopher behaviour |
| `forks.c` | fork allocation, acquisition and release |
| `meal_state.c` | synchronized meal state and starvation decision |
| `monitor.c` | death and meal-completion monitoring |
| `state.c` | global stop access |
| `log.c` | serialized event output |
| `messages.c` | usage and error messages |
| `time.c` | timestamps and responsive sleeping |
| `utils.c` | minimal parsing/string helpers |

For the concurrency details behind these components, see
[`synchronization.md`](synchronization.md).
