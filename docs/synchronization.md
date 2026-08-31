# Synchronization

## Overview

The concurrency model separates different kinds of shared state instead of
placing the entire simulation behind one global lock.

The implementation uses:

| Primitive | Protected responsibility |
|---|---|
| one mutex per fork | exclusive ownership of each fork |
| `start_mutex` | `ready_count` and `start` |
| `meal_mutex` | `last_meal_ms` and `meals_eaten` |
| `state_mutex` | global `stop` |
| `print_mutex` | serialized terminal output |

There are no condition variables in the maintained implementation.

## Fork ownership

Every fork is represented by one `pthread_mutex_t`.

A philosopher must own both neighboring fork mutexes before beginning a meal.

Fork acquisition order depends on philosopher ID:

    odd ID:
        left
        right

    even ID:
        right
        left

This prevents every philosopher from following the same circular acquisition
pattern.

The strategy is part of deadlock avoidance, but it should not be interpreted as
a formal fairness or starvation-freedom guarantee for arbitrary scheduling.

## Start synchronization

Thread creation itself takes time. Allowing the earliest thread to begin its
starvation timer while later threads are still being created would create an
artificial timing disadvantage.

Workers therefore enter a start gate.

### Worker side

Each philosopher:

    lock start_mutex
    ready_count++
    unlock start_mutex

and then repeatedly reads the protected `start` flag.

### Main-thread side

The main thread polls `ready_count` until every worker has reached the gate.

It then initializes the shared timing state before opening the gate.

Under `meal_mutex`:

    start_ms = now_ms()
    every philosopher.last_meal_ms = start_ms

Then, under `start_mutex`:

    start = 1

Workers can only leave their polling loop after this state has been
established.

The implementation uses short `usleep()` polling intervals rather than a
condition variable or POSIX barrier.

## Meal state

`last_meal_ms` and `meals_eaten` are modified together by
`update_meal_state()`:

    lock meal_mutex
    last_meal_ms = now_ms()
    meals_eaten++
    unlock meal_mutex

Meal-count reads also use the same mutex.

One shared `meal_mutex` serializes these operations across all philosophers.

## Atomic starvation decision

The most important post-baseline synchronization correction concerns
starvation detection.

Historically, the monitor obtained `last_meal_ms` under `meal_mutex`, released
the mutex and only then compared that saved timestamp with the current time.

That allowed the following logical interleaving:

    monitor                       philosopher

    lock meal
    read old last_meal
    unlock meal

                                  lock meal
                                  start new meal
                                  update last_meal
                                  unlock meal

    compare old last_meal
    declare death

Every individual data access could still be mutex-protected while the
higher-level decision operated on stale state.

The maintained implementation instead performs the decision in
`stop_if_starved()`:

    lock meal_mutex
    now = now_ms()
    dead = now - last_meal_ms >= t_die

    if dead
        set global stop

    unlock meal_mutex

A meal-start update therefore cannot occur between the protected timestamp read
and the death decision.

This is an example of why absence of low-level data-race reports does not by
itself prove that a multi-step logical state transition is synchronized
correctly.

## Death boundary

The maintained starvation condition is:

    elapsed >= time_to_die

This matches the deadline semantics directly: once the configured starvation
interval has been reached without a new meal start, the philosopher is dead.

The historical pre-maintenance implementation used a strict `>` comparison.

## Global stop state

`set_stop()` and `get_stop()` protect the global termination flag with
`state_mutex`.

Workers check the flag:

- before beginning actions;
- after potentially blocking fork acquisition;
- during responsive sleeps;
- while waiting for the start gate.

The monitor sets the same flag on starvation or after the optional meal target
has been reached by every philosopher.

## Output serialization

`log_philo()` holds `print_mutex` while deciding whether to emit and while
printing one complete event line.

Normal messages are printed only while the simulation is active.

The death message is explicitly allowed after the stop flag has been set:

    !stop || message == "died"

This prevents ordinary state transitions from being printed after termination
while still allowing the monitor to report the detected death.

## Lock relationships

The maintained code contains a few nested synchronization paths worth making
explicit.

Starvation detection may use:

    meal_mutex
        `-- state_mutex

Logging uses:

    print_mutex
        `-- state_mutex

A philosopher may also hold one or both fork mutexes while updating meal state
or logging fork acquisition.

No maintained path was identified that acquires these same domains in the
opposite direction while retaining the first lock.

Start initialization uses `meal_mutex` and `start_mutex` sequentially rather
than holding both simultaneously.

## Timing and stop responsiveness

`smart_sleep()` does not delegate the entire requested duration to one
`usleep()` call.

It records a start timestamp and repeatedly computes elapsed time:

    start = now_ms()

    while simulation is active
        elapsed = now_ms() - start
        stop when elapsed >= requested duration
        sleep for a short interval

This keeps sleeping workers responsive to global termination.

The elapsed-time formulation also avoids signed overflow from constructing a
future timestamp with `now_ms() + duration_ms` for very large accepted timing
arguments.

## Scheduling heuristics

Two small scheduling decisions reduce contention.

Even philosopher IDs wait for half of `t_eat` before entering their first
normal cycle.

For odd philosopher counts, `get_think_delay()` may add a bounded thinking
delay derived from the configured eat, sleep and death timings.

These are pragmatic contention-management mechanisms.

They are not documented as guarantees of scheduler fairness or proof that
starvation can never occur under every possible workload.

## Concurrency validation

The maintained runtime has been exercised with both Helgrind and DRD with zero
configured errors in the recorded validation runs.

Those results are useful evidence about data-race behaviour, but the
starvation-decision correction demonstrates why tool output must be combined
with reasoning about the atomicity of higher-level state transitions.

See [`testing.md`](testing.md) for the complete validation record.
