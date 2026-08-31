# Testing and validation

## Philosophy

Concurrency testing needs more than one successful example.

The maintained project has been checked across four complementary areas:

    build correctness
    runtime behaviour
    concurrency / memory tooling
    arithmetic and boundary behaviour

The goal of this document is to record observed evidence without presenting the
test matrix as a formal proof of correctness.

## Historical boundary

The annotated tag:

    portfolio-baseline-2026-08

targets:

    843a758825c6790cd2deb124f2b1d70cfffcb44e

and identifies the repository state before professional portfolio maintenance.

Post-baseline runtime maintenance was introduced through:

    #2 / PR #3  starvation-decision synchronization
    #4 / PR #5  timing and allocation arithmetic hardening

Repository cleanup followed through:

    #6 / PR #7

The cleanup verification demonstrated that the complete mandatory implementation
remained byte-for-byte unchanged after the last runtime-changing maintenance
state.

## Reference build

The project Makefile uses:

    CC = cc
    CFLAGS = -Wall -Wextra -Werror -pthread

Recorded reference-build validation:

    fclean:    0
    build:     0
    warnings:  0
    errors:    0
    result:    PASS

A repeated `make` was also checked to confirm that an already-current executable
was not relinked.

Result:

    no relink: PASS

## Compiler diversity

The maintained implementation was additionally built with Clang by overriding
the Makefile compiler interface:

    make CC=clang

Recorded result:

    build:     PASS
    warnings:  0
    errors:    0

The Makefile itself intentionally remains compiler-neutral through `CC = cc`.

## Runtime regression

The latest full runtime-changing maintenance state was exercised across 63
automated scenarios.

### Death timing

The matrix included repeated runs of:

    1 800 200 200
    2 310 200 100
    4 310 200 100

Observed death timestamps remained at the configured deadline or within the
millisecond-scale polling/scheduling boundary expected by the test.

Recorded subtotal:

    40 / 40 PASS

### Stable simulations

Repeated non-terminating observation windows included:

    4 410 200 200
    5 800 200 200

No death was observed during the configured validation windows.

Recorded subtotal:

    15 / 15 PASS

### Meal-limited simulations

Termination scenarios included:

    5 800 200 200 7
    5 610 200 200 10

Every philosopher reached the requested meal count without an observed death.

Recorded subtotal:

    8 / 8 PASS

### Runtime total

    PASS:  63
    FAIL:  0
    TOTAL: 63

A separate synchronization-focused validation performed immediately after the
starvation-decision correction executed 89 targeted scenarios with:

    PASS: 89
    FAIL: 0

The later 63-case regression is the relevant runtime matrix after the final
runtime-changing arithmetic maintenance.

## Historical regression scenario

Earlier testing had identified:

    5 610 200 200 10

as a scenario worth retaining as a regression case.

Repeated post-maintenance runs completed with every philosopher reaching ten
meals and no recorded death.

It therefore remains useful as a historical regression case even though the
original failure was not reproduced during the later baseline audit.

## Memory validation

Memcheck was run against the maintained implementation.

Recorded result:

    ERROR SUMMARY: 0 errors
    heap at exit:  0 bytes in 0 blocks

This validates the exercised execution path for leaks and configured memory
errors.

## Thread-analysis tools

### Helgrind

Recorded maintained-state result:

    ERROR SUMMARY: 0 errors

### DRD

Recorded maintained-state result:

    ERROR SUMMARY: 0 errors

These tools are used as evidence for the exercised synchronization paths, not as
a substitute for manual reasoning about compound state transitions.

The starvation-decision issue fixed after the historical baseline is an example:
individual accesses were mutex-protected, while the logical read-decide
operation still needed stronger synchronization.

## Static analysis

### Clang Static Analyzer

After the implementation-quality maintenance:

    warnings: 0
    errors:   0
    result:   PASS

### GCC analyzer

Recorded result:

    warnings: 0
    errors:   0
    result:   PASS

## Undefined-behaviour checks

UBSan was used for normal execution and for a deliberately extreme timing
argument.

The historical implementation of `smart_sleep()` constructed:

    end = now_ms() + duration_ms

With an accepted `LONG_MAX` duration this could overflow signed `long`.

The maintained implementation uses elapsed-time arithmetic instead.

Post-fix UBSan results:

    normal scenario:
        no configured diagnostic

    LONG_MAX duration:
        no configured diagnostic
        external timeout expected because the duration is intentionally huge

## Thinking-delay arithmetic

The odd-philosopher thinking-delay calculation was algebraically rewritten to
avoid potentially overflowing intermediate expressions involving twice the
eating duration.

The old mathematical model and the rewritten model were compared across:

    2,048,000 ordinary input combinations
    2,187 LONG_MAX-oriented edge combinations

Recorded result:

    all compared results equivalent

This checks behavioural equivalence of the arithmetic transformation over the
tested domain while avoiding the unsafe intermediate operations.

## Repository-cleanup preservation check

Before repository cleanup, SHA-256 hashes were recorded for every tracked file
under:

    Philosophers/philo/

The hashes were recomputed after removing the incomplete bonus scaffold,
redundant placeholders and obsolete ignore rules.

Recorded result:

    mandatory tree is byte-for-byte unchanged

The reference build and no-relink check were then repeated successfully.

## Representative manual commands

Build:

    cd Philosophers/philo
    make

Invalid CLI:

    ./philo
    ./philo foo 800 200 200

Single philosopher:

    ./philo 1 800 200 200

Stable simulation:

    ./philo 5 800 200 200

Meal-limited simulation:

    ./philo 5 800 200 200 7

Boundary scenario:

    ./philo 4 410 200 200

Expected-death scenario:

    ./philo 4 310 200 100

## Current quality claims

The maintained portfolio documentation supports claims about the validation
listed above.

It does not claim that:

    testing proves correctness for every possible scheduler interleaving
    the implementation is starvation-free under arbitrary workloads
    the bonus project is implemented
    the current maintained source is Norminette-compliant

Those are deliberately outside the evidence recorded for the maintained
repository.
