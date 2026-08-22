#!/usr/bin/env python3
"""Checks the responses collected from concurrent requests to /calc.

Every response must carry the square of its own number, the upper-cased text,
and the per-thread call counter of the main program must equal the counter of
the CALLed subprogram. Within one request thread the counters must be a
contiguous sequence (requests sent before the measurement, such as the
readiness probe, may have consumed the first values), which shows that each
thread has its own WORKING-STORAGE.
"""
import collections
import re
import sys

pattern = re.compile(
    r"num=(\d+) square=(\d+) upper=(\S+)\s+a=(\d+) calls=(\d+) subcalls=(\d+) thread=(\S+)"
)
bad = 0
per_thread = collections.defaultdict(list)
total = 0
for line in open(sys.argv[1], encoding="ascii", errors="replace"):
    line = line.strip()
    if not line:
        continue
    total += 1
    m = pattern.search(line)
    if not m:
        bad += 1
        print("unexpected response:", line)
        continue
    num, square, upper, a, calls, subcalls, thread = m.groups()
    per_thread[thread].append(int(calls))
    if int(square) != int(num) ** 2 or upper != "BANANA" or a != "03" or calls != subcalls:
        bad += 1
        print("wrong response:", line)
for thread, counters in per_thread.items():
    counters.sort()
    if counters != list(range(counters[0], counters[0] + len(counters))):
        bad += 1
        print("counter of", thread, "is not contiguous:", counters)
print("responses", total, "threads", len(per_thread), "bad", bad)
sys.exit(1 if bad or total == 0 else 0)
