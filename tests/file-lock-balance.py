#!/usr/bin/env python3
"""Find the optimal way to split the file lock tests into N CI jobs.

This is the multiprocessor scheduling problem (P||Cmax): assign each test to
one of N groups so that the largest group total is as small as possible.

With 11 tests and 4 groups the search space is small enough (4^11, minus
symmetric duplicates) to enumerate exhaustively, so the answer is provably
optimal rather than a heuristic such as longest-processing-time-first.

Run times below were measured in CI (run 30346475956) by taking the
difference between the GitHub Actions timestamps of consecutive autotest
result lines, so they include compilation and JVM startup exactly as the
CI jobs see them.

Usage:
    python3 file-lock-balance.py [--groups N] [--platform NAME]
"""

import argparse
import itertools

# seconds, measured in CI
TIMES = {
    # test (.at file)             linux  linux-utf8  windows
    "lock-file": {"linux": 135.8, "linux-utf8": 145.2, "windows": 248.5},
    "lock-mode-clause": {"linux": 119.4, "linux-utf8": 123.6, "windows": 191.8},
    "access-different-record": {"linux": 62.7, "linux-utf8": 63.6, "windows": 187.5},
    "access-same-record": {"linux": 59.5, "linux-utf8": 62.9, "windows": 121.3},
    "lock-mode-automatic": {"linux": 63.4, "linux-utf8": 60.6, "windows": 96.8},
    "open-start-write-rewrite": {"linux": 46.7, "linux-utf8": 44.6, "windows": 72.0},
    "input-mode": {"linux": 46.8, "linux-utf8": 44.6, "windows": 65.0},
    "open-input": {"linux": 22.3, "linux-utf8": 28.7, "windows": 33.4},
    "release-lock": {"linux": 23.7, "linux-utf8": 24.4, "windows": 30.5},
    "same-process": {"linux": 1.2, "linux-utf8": 1.3, "windows": 1.4},
    "old-file": {"linux": 1.0, "linux-utf8": 1.2, "windows": 1.4},
}

PLATFORMS = ["linux", "linux-utf8", "windows"]


def partitions(tests, n):
    """Enumerate every assignment of tests to n groups, skipping the
    relabelings of a partition we have already produced."""
    for assign in itertools.product(range(n), repeat=len(tests)):
        if len(set(assign)) < n:
            continue  # an empty group is never useful here
        first_seen = {}
        for group in assign:
            if group not in first_seen:
                first_seen[group] = len(first_seen)
        if [first_seen[g] for g in assign] != list(assign):
            continue  # symmetric duplicate
        yield assign


def totals(tests, assign, n, platform):
    out = [0.0] * n
    for test, group in zip(tests, assign):
        out[group] += TIMES[test][platform]
    return out


def solve(n, platforms):
    """Minimize the largest group total.

    A single platform minimizes its makespan directly. Several platforms have
    different totals, so each one's makespan is divided by its own perfect
    split (total / n) and the worst ratio is minimized -- otherwise the
    slowest platform would be the only one that counts.
    """
    tests = sorted(TIMES, key=lambda t: -TIMES[t][platforms[-1]])
    perfect = {p: sum(TIMES[t][p] for t in TIMES) / n for p in platforms}
    best = None
    for assign in partitions(tests, n):
        worst = max(
            max(totals(tests, assign, n, p)) / perfect[p] for p in platforms
        )
        # tie-break on spread so the runners-up are balanced too
        spread = sum(
            (x - perfect[p]) ** 2
            for p in platforms
            for x in totals(tests, assign, n, p)
        )
        if best is None or (worst, spread) < best[0]:
            best = ((worst, spread), assign)
    groups = [
        [t for t, g in zip(tests, best[1]) if g == i] for i in range(n)
    ]
    return groups, best[0][0], perfect


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--groups", type=int, default=4)
    parser.add_argument("--platform", choices=PLATFORMS)
    args = parser.parse_args()
    platforms = [args.platform] if args.platform else PLATFORMS

    groups, ratio, perfect = solve(args.groups, platforms)
    print(f"optimizing for: {', '.join(platforms)}")
    print(f"perfect split:  " + "  ".join(f"{p}={perfect[p]:.0f}s" for p in platforms))
    print(f"worst group is {ratio:.3f}x its perfect split\n")
    for i, group in enumerate(groups, 1):
        times = {p: sum(TIMES[t][p] for t in group) for p in platforms}
        print(f"group {i}: " + ", ".join(group))
        print("         " + "  ".join(f"{p}={times[p]:.0f}s" for p in platforms))


if __name__ == "__main__":
    main()
