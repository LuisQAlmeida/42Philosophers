# Final portfolio audit

Audit date: 2026-09-01

Audited maintained source:

    12b59275a05e17227b5fae0ed0ccb96d1e3eebba

Historical pre-modernization baseline:

    portfolio-baseline-2026-08
    843a758825c6790cd2deb124f2b1d70cfffcb44e

## Verdict

**READY FOR RELEASE-CANDIDATE VALIDATION**

No known P0 or P1 correctness finding remains open from the completed
portfolio-modernization audit.

This verdict means that the maintained repository is ready for validation of
the exact final `main` commit that may become `v1.0.0`.

It does not mean that every possible scheduling interleaving has been formally
proven correct.

## Audit scope

The final audit reviewed:

- Git provenance and historical preservation;
- repository structure and hygiene;
- public documentation;
- maintained mandatory source;
- build reproducibility;
- compiler diversity;
- repository-owned regression testing;
- undefined-behaviour diagnostics;
- static analysis;
- memory behaviour;
- concurrency diagnostics;
- repeated runtime stability;
- death timing and output ordering;
- higher-thread-count behaviour;
- release-readiness boundaries.

The audit was performed on the dedicated issue branch for #12 while that branch
still pointed exactly to maintained `main`.

## Historical preservation

The annotated tag:

    portfolio-baseline-2026-08

continues to resolve to:

    843a758825c6790cd2deb124f2b1d70cfffcb44e

That commit represents the repository state before professional portfolio
modernization.

The baseline is an ancestor of the audited maintained state and was not
rewritten by the modernization work.

The incomplete historical bonus scaffold remains recoverable through that
baseline tag. It is intentionally absent from the maintained tree and is not
presented as a completed bonus implementation.

## Post-baseline maintenance history

The audited maintained state contains five focused post-baseline changes:

    8a5b70b  fix(philo): synchronize starvation decision (#3)
    079cb2b  fix(philo): harden timing and allocation arithmetic (#5)
    bcd08b7  chore(repo): clean maintained project structure (#7)
    ea97ec2  docs(portfolio): document maintained Philosophers (#9)
    12b5927  test(ci): add reproducible regression validation (#11)

These commits remain distinguishable from the historical implementation.

## Repository state

At the audited commit the maintained repository contained:

    tracked files:       27
    Markdown files:       8
    C source files:      13
    C header files:       1
    workflow files:       1
    regression runners:   1

Repository-hygiene checks found:

    tracked placeholders:             none
    tracked generated artefacts:      none
    tracked backup artefacts:         none
    tracked core dumps:               none
    untracked non-ignored artefacts:  none

The repository-owned regression runner retains executable mode.

## Documentation audit

The maintained public documentation consists of:

- `README.md`;
- `Philosophers/README.md`;
- `docs/architecture.md`;
- `docs/synchronization.md`;
- `docs/testing.md`;
- two retained learning-resource notes;
- `resources/tutorials/tutorial_links.md`.

The final local Markdown audit inspected eight Markdown files and resolved all
16 local links successfully.

The documentation distinguishes the historical implementation from later
portfolio maintenance.

It also avoids claims that are not supported by the implementation or the
available evidence.

In particular, the maintained documentation does not claim:

- completion of the historical bonus project;
- starvation freedom for arbitrary scheduler behaviour;
- formal proof of concurrency correctness;
- current Norminette compliance;
- production-shell or general-purpose concurrency-library scope.

## Maintained synchronization model

The maintained mandatory implementation uses:

- one mutex per fork;
- `start_mutex` for coordinated startup state;
- `meal_mutex` for meal timestamps and meal counters;
- `state_mutex` for the global stop state;
- `print_mutex` for serialized output.

The post-baseline synchronization correction made the starvation decision and
the meal-start timestamp update operate within the same meal-state
synchronization domain.

The maintained starvation boundary is:

    elapsed >= time_to_die

The audit found no project-defined global mutable data symbol.

## Build validation

The reference build completed successfully using the maintained Makefile
interface:

    CC = cc
    CFLAGS = -Wall -Wextra -Werror -pthread

Observed result:

    reference build:      PASS
    compiler warnings:    0
    compiler errors:      0
    repeated make:        PASS
    unnecessary relink:   no

Compiler-diversity validation using:

    make CC=clang

also completed with:

    warnings: 0
    errors:   0

## Repository-owned regression validation

The default repository-owned regression suite completed:

    PASS:   8
    FAIL:   0
    TOTAL:  8

The extended local suite completed:

    PASS:   9
    FAIL:   0
    TOTAL:  9

The extended suite includes the historical:

    5 610 200 200 10

scenario.

The regression runner was also executed against `/bin/true` as a negative
control.

Result:

    PASS: runner rejected the deliberately incorrect executable

This confirms that failed expectations propagate as suite failure instead of
being reported as unconditional success.

## Repeated runtime stability

The complete default regression suite was repeated five times:

    5 PASS
    0 FAIL

The complete extended suite was repeated three times:

    3 PASS
    0 FAIL

No intermittent failure was observed during those repetitions.

## Death timing and output stress

A separate stress matrix executed 50 expected-death scenarios.

Result:

    PASS:   50
    FAIL:    0
    TOTAL:  50

Observed death timing:

    1 800 200 200
    range: 800-800 ms

    2 310 200 100
    range: 310-311 ms

    4 310 200 100
    range: 310-311 ms

Every accepted run contained exactly one death event, no event after the death
line, valid philosopher identifiers and valid output syntax.

## Higher-thread-count smoke validation

A bounded run using 50 philosophers completed successfully.

Observed result:

    exit status:   0
    death lines:   0
    invalid lines: 0
    output lines:  424

This is a representative smoke check rather than a scalability guarantee.

## Static analysis

Clang Static Analyzer completed over all maintained C translation units with:

    warnings: 0
    errors:   0

GCC `-fanalyzer` completed with:

    warnings: 0
    errors:   0

No release-gating static-analysis finding remained open.

## Undefined-behaviour validation

A Clang UBSan build completed with:

    warnings: 0
    errors:   0

The default regression matrix completed under UBSan:

    PASS: 8 / 8
    UBSan diagnostics: 0

An intentionally extreme `LONG_MAX` timing input was also exercised.

Result:

    expected external timeout: yes
    UBSan diagnostics:         0

This specifically rechecks the timing-arithmetic hardening introduced after
the historical baseline.

## Memory validation

Valgrind Memcheck completed with:

    in use at exit: 0 bytes in 0 blocks
    total heap usage: 8 allocs, 8 frees
    ERROR SUMMARY: 0 errors

No leak was observed in the representative audited scenario.

## Concurrency diagnostics

Valgrind Helgrind completed with:

    ERROR SUMMARY: 0 errors

Valgrind DRD completed with:

    ERROR SUMMARY: 0 errors

These results provide dynamic evidence for the exercised scenarios. They are
not a formal proof covering every scheduler interleaving.

## Norminette context

The installed Norminette reported:

    INVALID_HEADER: 14

The findings correspond to the maintained 13 C source files and one maintained
header lacking the historical 42 file banner.

The maintained portfolio explicitly makes no claim of current Norminette
compliance.

The result is therefore retained as tooling context rather than classified as
a release-gating correctness finding.

The historical source state remains recoverable through the immutable
portfolio baseline.

## Automated validation

The repository contains:

    .github/workflows/ci.yml

with two focused jobs:

    CI / build
    CI / quality

The build job validates:

- the reference build;
- executable creation;
- repeated-make no-relink behaviour;
- the default repository-owned regression suite.

The quality job validates:

- an Ubuntu 24.04 runner;
- Clang availability;
- compiler-diversity build through `make CC=clang`.

The maintained Makefile is not rewritten for CI.

## Finding classification

At completion of the final local audit:

    P0 release-gating findings: 0
    P1 release-gating findings: 0

One informational tooling condition remains documented:

    current Norminette reports INVALID_HEADER because the maintained
    portfolio state does not retain the historical 42 file banners

No source change is required solely to convert that informational result into
a green Norminette report.

## Release-readiness decision

The audited maintained source is accepted as:

**READY FOR RELEASE-CANDIDATE VALIDATION**

Before an annotated `v1.0.0` tag is created, the exact final `main` commit must
still satisfy the release-candidate gate:

- local `main` must exactly match `origin/main`;
- hosted CI checks must pass;
- the repository-owned regression suite must pass;
- reference and compiler-diversity builds must pass;
- the historical baseline must remain unchanged;
- no generated artefact may be tracked;
- the worktree must be clean;
- the final validated commit must be recorded before tagging.

The eventual annotated `v1.0.0` tag must point to that exact validated commit.

A matching GitHub Release must use the same tag and preserve the distinction
between the historical project state and later maintained portfolio work.
