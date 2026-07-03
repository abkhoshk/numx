# Hardware Profile: Windows x64 (MSVC)

**Profile date:** 2026-06-05 (float32) / 2026-06-06 (float64)
**numx commit:** 4c4c0f0 (float32 run) / 1bba399 (float64 run)

## System

| Field        | Value                                              |
|--------------|----------------------------------------------------|
| Board/host   | Windows 11 desktop                                 |
| OS           | Windows 11 Pro (build 10.0.26200)                  |
| CPU          | — (not recorded)                                   |
| Architecture | x86-64                                             |
| Compiler     | MSVC 14.51 (VS 2026 Build Tools)                   |
| C standard   | C99 (`/std:c11`)                                   |
| Optimisation | `/O2`                                              |
| Timer        | `QueryPerformanceCounter`                          |
| Precision    | float32 (first run) and float64 / `NUMX_USE_DOUBLE` (second run) |

## Notes

- Two validation runs: float32 (default, no `NUMX_USE_DOUBLE`, commit 4c4c0f0,
  2026-06-05) and float64 (`NUMX_USE_DOUBLE` defined, commit 1bba399, 2026-06-06).
- Some Unity assertions use float32-level tolerances even in the float64 build.
  Affected tests still pass because errors are well within float32 tolerance.
  This is a test harness configuration issue, not a library bug. See individual
  result files for per-module detail.
- Benchmarks run in a Windows user-mode process; no CPU affinity or real-time
  priority. Results are indicative; expect ±10% variance between runs.

## NTT addendum (2026-07-03)

Two additional runs, commit `daf5b9c`, validator Amir Ab Khoshk:

- **Windows x64 / float64** (this profile's existing config): 29 new NTT tests added.
  `validation/results/ntt/ntt.md` reports 329/329 total; Amir confirmed this by
  re-running the exact `NUMX_USE_DOUBLE` build, 329 Tests / 0 Failures / 0 Ignored.
  This supersedes the 294-test baseline in the header above — that count is stale
  (the non-NTT test total on this platform grew to 300 sometime after the 2026-06-06
  run, independent of NTT, matching the 300-test baseline everywhere else).
- **Windows x86 (32-bit, i386)**: a new architecture variant not otherwise covered by
  this profile — built with the Win32 CMake generator (`-A Win32`), confirmed 32-bit via
  `file`. 329/329 tests pass (first full-suite run on this architecture). No benchmark
  numbers: `benchmarks/bench_win.c` doesn't call `numx_bench_ntt()`.

Full detail: [`validation/results/ntt/ntt.md`](../results/ntt/ntt.md).
