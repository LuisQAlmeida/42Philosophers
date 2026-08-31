#!/usr/bin/env python3

from __future__ import annotations

import argparse
import re
import subprocess
import sys
import time
from collections import Counter
from dataclasses import dataclass
from pathlib import Path
from typing import Sequence


REPO_ROOT = Path(__file__).resolve().parents[1]
DEFAULT_BINARY = REPO_ROOT / "Philosophers" / "philo" / "philo"

EVENT_RE = re.compile(
    r"^(?P<ms>\d+)\s+(?P<id>\d+)\s+(?P<state>.+)$"
)

VALID_STATES = {
    "has taken a fork",
    "is eating",
    "is sleeping",
    "is thinking",
    "died",
}


@dataclass
class ProcessResult:
    timed_out: bool
    returncode: int | None
    elapsed: float
    output: str


@dataclass
class Event:
    timestamp_ms: int
    philosopher_id: int
    state: str


@dataclass
class TestResult:
    name: str
    passed: bool
    elapsed: float
    details: str = ""


def decode_timeout_output(value: str | bytes | None) -> str:
    if value is None:
        return ""
    if isinstance(value, bytes):
        return value.decode(errors="replace")
    return value


def run_process(
    binary: Path,
    args: Sequence[object],
    timeout: float,
) -> ProcessResult:
    started = time.monotonic()

    try:
        proc = subprocess.run(
            [str(binary), *map(str, args)],
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            text=True,
            timeout=timeout,
            check=False,
        )

        return ProcessResult(
            timed_out=False,
            returncode=proc.returncode,
            elapsed=time.monotonic() - started,
            output=proc.stdout or "",
        )

    except subprocess.TimeoutExpired as exc:
        return ProcessResult(
            timed_out=True,
            returncode=None,
            elapsed=time.monotonic() - started,
            output=decode_timeout_output(exc.stdout),
        )


def parse_events(
    output: str,
    philosopher_count: int,
) -> tuple[list[Event], list[str]]:
    parsed: list[Event] = []
    invalid: list[str] = []

    for raw in output.splitlines():
        line = raw.strip()

        if not line:
            continue

        match = EVENT_RE.fullmatch(line)

        if match is None:
            invalid.append(line)
            continue

        event = Event(
            timestamp_ms=int(match.group("ms")),
            philosopher_id=int(match.group("id")),
            state=match.group("state"),
        )

        if not 1 <= event.philosopher_id <= philosopher_count:
            invalid.append(line)
            continue

        if event.state not in VALID_STATES:
            invalid.append(line)
            continue

        parsed.append(event)

    return parsed, invalid


def death_state(
    events: Sequence[Event],
) -> tuple[int | None, int, int]:
    death_indexes = [
        index
        for index, event in enumerate(events)
        if event.state == "died"
    ]

    if not death_indexes:
        return None, 0, 0

    first = death_indexes[0]

    return (
        events[first].timestamp_ms,
        len(death_indexes),
        len(events[first + 1:]),
    )


def meal_counts(events: Sequence[Event]) -> dict[int, int]:
    counts = Counter(
        event.philosopher_id
        for event in events
        if event.state == "is eating"
    )

    return dict(sorted(counts.items()))


def make_result(
    name: str,
    passed: bool,
    process: ProcessResult,
    details: str = "",
) -> TestResult:
    return TestResult(
        name=name,
        passed=passed,
        elapsed=process.elapsed,
        details=details,
    )


def test_missing_arguments(binary: Path) -> TestResult:
    process = run_process(binary, [], 1.0)

    passed = (
        not process.timed_out
        and process.returncode is not None
        and process.returncode != 0
        and "Invalid arguments." in process.output
        and "Usage:" in process.output
    )

    return make_result(
        "missing arguments",
        passed,
        process,
        f"rc={process.returncode}",
    )


def test_non_numeric(binary: Path) -> TestResult:
    process = run_process(
        binary,
        ["foo", 800, 200, 200],
        1.0,
    )

    passed = (
        not process.timed_out
        and process.returncode is not None
        and process.returncode != 0
        and "Invalid arguments." in process.output
    )

    return make_result(
        "non-numeric argument",
        passed,
        process,
        f"rc={process.returncode}",
    )


def test_zero_philosophers(binary: Path) -> TestResult:
    process = run_process(
        binary,
        [0, 800, 200, 200],
        1.0,
    )

    passed = (
        not process.timed_out
        and process.returncode is not None
        and process.returncode != 0
        and "Invalid arguments." in process.output
    )

    return make_result(
        "zero philosopher count",
        passed,
        process,
        f"rc={process.returncode}",
    )


def expected_death_test(
    binary: Path,
    name: str,
    args: Sequence[int],
    philosopher_count: int,
    time_to_die: int,
    timeout: float,
) -> TestResult:
    process = run_process(binary, args, timeout)

    events, invalid = parse_events(
        process.output,
        philosopher_count,
    )

    death_ms, death_count, post_death = death_state(events)

    passed = (
        not process.timed_out
        and process.returncode == 0
        and not invalid
        and death_count == 1
        and death_ms is not None
        and death_ms >= time_to_die
        and post_death == 0
    )

    return make_result(
        name,
        passed,
        process,
        (
            f"death={death_ms}ms "
            f"death_lines={death_count} "
            f"post_death={post_death} "
            f"invalid_lines={len(invalid)}"
        ),
    )


def stable_window_test(
    binary: Path,
    name: str,
    args: Sequence[int],
    philosopher_count: int,
    observation_window: float,
) -> TestResult:
    process = run_process(
        binary,
        args,
        observation_window,
    )

    events, invalid = parse_events(
        process.output,
        philosopher_count,
    )

    _, death_count, _ = death_state(events)

    passed = (
        process.timed_out
        and not invalid
        and death_count == 0
    )

    return make_result(
        name,
        passed,
        process,
        (
            f"survival_window={observation_window:.1f}s "
            f"events={len(events)} "
            f"death_lines={death_count} "
            f"invalid_lines={len(invalid)}"
        ),
    )


def meal_limited_test(
    binary: Path,
    name: str,
    args: Sequence[int],
    philosopher_count: int,
    meal_target: int,
    timeout: float,
) -> TestResult:
    process = run_process(
        binary,
        args,
        timeout,
    )

    events, invalid = parse_events(
        process.output,
        philosopher_count,
    )

    _, death_count, post_death = death_state(events)
    meals = meal_counts(events)

    all_reached = (
        len(meals) == philosopher_count
        and all(
            meals.get(philosopher_id, 0) >= meal_target
            for philosopher_id
            in range(1, philosopher_count + 1)
        )
    )

    passed = (
        not process.timed_out
        and process.returncode == 0
        and not invalid
        and death_count == 0
        and post_death == 0
        and all_reached
    )

    return make_result(
        name,
        passed,
        process,
        (
            f"meals={meals} "
            f"death_lines={death_count} "
            f"invalid_lines={len(invalid)}"
        ),
    )


def build_test_plan(binary: Path, extended: bool):
    tests = [
        lambda: test_missing_arguments(binary),
        lambda: test_non_numeric(binary),
        lambda: test_zero_philosophers(binary),

        lambda: expected_death_test(
            binary,
            "single philosopher starvation",
            [1, 200, 60, 60],
            philosopher_count=1,
            time_to_die=200,
            timeout=2.0,
        ),

        lambda: expected_death_test(
            binary,
            "expected multi-philosopher death",
            [4, 310, 200, 100],
            philosopher_count=4,
            time_to_die=310,
            timeout=2.0,
        ),

        lambda: stable_window_test(
            binary,
            "410 ms boundary survival",
            [4, 410, 200, 200],
            philosopher_count=4,
            observation_window=1.2,
        ),

        lambda: stable_window_test(
            binary,
            "stable five philosophers",
            [5, 800, 200, 200],
            philosopher_count=5,
            observation_window=1.2,
        ),

        lambda: meal_limited_test(
            binary,
            "meal-limited termination",
            [5, 800, 200, 200, 3],
            philosopher_count=5,
            meal_target=3,
            timeout=6.0,
        ),
    ]

    if extended:
        tests.append(
            lambda: meal_limited_test(
                binary,
                "historical 5 610 200 200 10",
                [5, 610, 200, 200, 10],
                philosopher_count=5,
                meal_target=10,
                timeout=10.0,
            )
        )

    return tests


def print_result(result: TestResult) -> None:
    status = "PASS" if result.passed else "FAIL"

    print(
        f"{status:<4}  "
        f"{result.name:<36} "
        f"{result.elapsed:>6.3f}s"
    )

    if result.details:
        print(f"      {result.details}")


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description=(
            "Run the maintained Philosophers regression suite."
        )
    )

    parser.add_argument(
        "--binary",
        type=Path,
        default=DEFAULT_BINARY,
        help=(
            "Path to the philo executable "
            f"(default: {DEFAULT_BINARY})"
        ),
    )

    parser.add_argument(
        "--extended",
        action="store_true",
        help=(
            "Include timing-sensitive historical regression cases "
            "kept outside the mandatory CI gate."
        ),
    )

    return parser.parse_args()


def main() -> int:
    args = parse_args()
    binary = args.binary.resolve()

    if not binary.is_file():
        print(
            f"ERROR: executable does not exist: {binary}",
            file=sys.stderr,
        )
        return 2

    if not binary.stat().st_mode & 0o111:
        print(
            f"ERROR: file is not executable: {binary}",
            file=sys.stderr,
        )
        return 2

    print("=== PHILOSOPHERS REGRESSION SUITE ===")
    print(f"binary: {binary}")
    print()

    results = []

    for test in build_test_plan(binary, args.extended):
        result = test()
        results.append(result)
        print_result(result)

    passed = sum(result.passed for result in results)
    failed = len(results) - passed
    runtime = sum(result.elapsed for result in results)

    print()
    print("=== REGRESSION SUMMARY ===")
    print(f"PASS: {passed}")
    print(f"FAIL: {failed}")
    print(f"TOTAL: {len(results)}")
    print(f"measured cumulative runtime: {runtime:.3f}s")

    if failed:
        print("RESULT: FAIL")
        return 1

    print("RESULT: PASS")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
