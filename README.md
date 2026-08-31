# Philosophers

A multithreaded C implementation of the Dining Philosophers concurrency
problem, developed as part of the 42 Common Core.

The project models each philosopher as an independent POSIX thread competing
for shared forks represented by mutexes. The implementation focuses on resource
ownership, synchronization, timing, starvation detection and coordinated
shutdown.

This repository also preserves the original completed project state before
later portfolio maintenance through the annotated historical tag
[`portfolio-baseline-2026-08`](https://github.com/LuisQAlmeida/42Philosophers/tree/portfolio-baseline-2026-08).

## What the project demonstrates

The maintained mandatory implementation exercises:

- POSIX thread creation and joining;
- mutex-protected shared resources;
- asymmetric fork acquisition;
- coordinated thread startup;
- synchronized simulation state;
- starvation and completion monitoring;
- serialized output;
- timing-sensitive concurrent behaviour;
- explicit resource cleanup.

The maintained implementation is located in
[`Philosophers/philo/`](Philosophers/philo/).

## Architecture at a glance

The program uses one worker thread per philosopher while the main thread owns
initialization, monitoring, thread joining and final cleanup.

    main
      |
      +-- parse rules
      +-- initialize shared mutexes
      +-- initialize fork mutexes
      +-- create philosopher threads
      +-- wait until all workers are ready
      +-- establish the shared start timestamp
      +-- release the start gate
      +-- monitor starvation / meal completion
      +-- join philosopher threads
      `-- release allocated resources

Each philosopher follows the same basic lifecycle:

    wait for start
          |
    optional initial stagger
          |
          v
        eat
          |
        sleep
          |
        think
          |
          `---- repeat until stop

The detailed execution model is documented in
[`docs/architecture.md`](docs/architecture.md).

## Synchronization model

The implementation separates shared state into several synchronization domains.

| Resource or state | Synchronization |
|---|---|
| Physical forks | one mutex per fork |
| Global stop flag | `state_mutex` |
| Meal timestamps and meal counts | `meal_mutex` |
| Start readiness and start gate | `start_mutex` |
| Terminal output | `print_mutex` |

Fork acquisition is asymmetric: odd philosopher IDs acquire their left fork
first, while even IDs acquire their right fork first. This prevents every
philosopher from following the same circular acquisition pattern.

Thread startup is coordinated through a readiness counter and a protected start
flag. Workers report that they are ready and wait until the main thread has
initialized the common simulation timestamp and each philosopher's initial meal
timestamp.

Starvation evaluation is serialized with meal-state updates. The monitor checks
the death deadline while holding `meal_mutex`, preventing a philosopher from
starting a new meal between the protected state read and the starvation
decision.

The complete locking model is documented in
[`docs/synchronization.md`](docs/synchronization.md).

## Build

From the mandatory implementation directory:

    cd Philosophers/philo
    make

The Makefile builds with:

    cc -Wall -Wextra -Werror -pthread

The resulting executable is:

    ./philo

Available cleanup targets are:

    make clean
    make fclean
    make re

## Usage

    ./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]

All arguments must be positive decimal integers.

`number_of_philosophers` and the optional meal target are limited to values
representable by `int`. Timing arguments are parsed as positive `long` values.

A few representative scenarios are:

    ./philo 1 800 200 200
    ./philo 5 800 200 200
    ./philo 5 800 200 200 7
    ./philo 4 410 200 200
    ./philo 4 310 200 100

Output follows the form:

    timestamp_ms philosopher_id state

Possible states are:

    has taken a fork
    is eating
    is sleeping
    is thinking
    died

## Monitoring and termination

The main thread acts as the simulation monitor after releasing the workers.

For each philosopher, starvation is detected when:

    current_time - last_meal_time >= time_to_die

When the optional meal target is supplied, the monitor also stops the
simulation once every philosopher has reached that number of meals.

After the global stop flag is set, normal state messages are suppressed while
the detected death message is still allowed to be printed.

## Timing strategy

`smart_sleep()` measures elapsed time relative to the beginning of the requested
sleep and periodically checks the global stop flag.

The maintained implementation deliberately avoids constructing a future
timestamp using:

    now + duration

because accepted timing values may extend up to `LONG_MAX`.

Even philosopher IDs receive an initial stagger of half the eating duration.
For odd philosopher counts, an additional bounded thinking delay may be used
when the configured timing margin makes it useful.

These mechanisms manage contention; they are not presented as a formal
starvation-freedom guarantee.

## Validation

The last runtime-changing maintenance state was validated with both the
reference compiler interface and Clang.

Recorded maintained-state validation includes:

    reference build:       PASS, 0 warnings, 0 errors
    Clang build:           PASS, 0 warnings, 0 errors
    repeated make:         PASS, no relink
    runtime regression:    63 / 63 PASS
    Clang analyzer:        0 findings
    GCC analyzer:          0 findings
    UBSan:                 no configured diagnostics
    Helgrind:              0 errors
    DRD:                   0 errors
    Memcheck:              0 errors, 0 bytes at exit

The repository cleanup that followed verified that the complete mandatory
implementation remained byte-for-byte unchanged.

Detailed scenarios, tool scope and interpretation are recorded in
[`docs/testing.md`](docs/testing.md).

The repository also includes a lightweight automated regression suite:

    ./tests/regression.py

GitHub Actions runs the reference build, no-relink check, regression suite and
a separate Clang compiler-diversity build on maintained integration changes.

## Repository structure

    .
    |-- Philosophers/
    |   |-- README.md
    |   `-- philo/
    |       |-- Makefile
    |       |-- include/
    |       `-- src/
    |-- docs/
    |   |-- architecture.md
    |   |-- synchronization.md
    |   `-- testing.md
    |-- resources/
    |   `-- tutorials/
    |       |-- tutorial_links.md
    |       `-- notes/
    |-- .gitignore
    |-- LICENSE
    `-- README.md

## Historical baseline and maintained state

The tag `portfolio-baseline-2026-08` points to the repository state immediately
before professional portfolio maintenance.

Its target is:

    843a758825c6790cd2deb124f2b1d70cfffcb44e

Post-baseline maintenance is intentionally visible in normal Git history.

The principal changes were:

- synchronization of starvation decisions with meal-start updates;
- correction of the death deadline from `>` to `>=`;
- overflow-safe timing arithmetic;
- explicit allocation-size handling;
- removal of incomplete and redundant repository scaffolding.

The historical tag remains immutable so the original project state and later
maintenance remain distinguishable.

## Scope

This repository presents the mandatory thread-and-mutex implementation.

The bonus implementation is not claimed as completed. An incomplete historical
bonus scaffold was removed from the maintained tree and remains inspectable
through `portfolio-baseline-2026-08`.

The project is a focused implementation of the 42 Philosophers requirements,
not a general-purpose concurrency library or proof of starvation-free
scheduling under arbitrary workloads.

No claim of current Norminette compliance is made by the maintained portfolio
documentation.

## Learning material

Development-era tutorial references and personal study notes are retained under
[`resources/tutorials/`](resources/tutorials/).

They are learning material rather than authoritative implementation
documentation. The maintained technical documentation under [`docs/`](docs/)
and the source code describe the current repository state.

## License

This repository is distributed under the terms of the
[MIT License](LICENSE).
