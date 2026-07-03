# Validation: numx_ntt

Covers: `numx_ntt_forward` · `numx_ntt_inverse` · `numx_ntt_pointwise_mul` · `numx_ntt_polymul` · `numx_ntt_poly_add` · `numx_ntt_poly_sub` · `numx_ntt_reduce`

---

## Windows x86 - Windows 11 / MSVC 19.51.36246.0 (VS 2026 Build Tools) / float32
**Validator:** Amir Ab Khoshk | **Date:** 2026-07-03 | **Commit:** daf5b9c

> Platform note: this run was performed on Windows/MSVC, not Linux x86-64. Raspberry Pi 4
> and ESP32-S3 validation were not performed in this session, no direct access to that
> hardware was available.
>
> Built with the Win32 CMake generator platform (`-A Win32`), producing a 32-bit
> Intel i386 binary, confirmed via `file build_x86/Release/numx_tests.exe`.
> Uses `numx_real_t = float` (no `NUMX_USE_DOUBLE`).

### Test cases

| Test | Result |
|------|--------|
| test_ntt_forward_delta | ✅ |
| test_ntt_forward_zero | ✅ |
| test_ntt_forward_null | ✅ |
| test_ntt_inverse_roundtrip_delta | ✅ |
| test_ntt_inverse_roundtrip_x | ✅ |
| test_ntt_inverse_roundtrip_x2 | ✅ |
| test_ntt_inverse_roundtrip_full | ✅ |
| test_ntt_inverse_null | ✅ |
| test_ntt_pointwise_mul_identity | ✅ |
| test_ntt_pointwise_mul_known | ✅ |
| test_ntt_pointwise_mul_null | ✅ |
| test_ntt_polymul_delta_identity | ✅ |
| test_ntt_polymul_x_times_x | ✅ |
| test_ntt_polymul_wrap_negacyclic | ✅ |
| test_ntt_polymul_x255_times_x | ✅ |
| test_ntt_polymul_commutativity | ✅ |
| test_ntt_polymul_known_linear | ✅ |
| test_ntt_polymul_random_vs_ref | ✅ |
| test_ntt_polymul_null | ✅ |
| test_ntt_reduce_noop_in_range | ✅ |
| test_ntt_reduce_boundary | ✅ |
| test_ntt_reduce_null | ✅ |
| test_ntt_poly_add_basic | ✅ |
| test_ntt_poly_add_wrap | ✅ |
| test_ntt_poly_add_null | ✅ |
| test_ntt_poly_sub_basic | ✅ |
| test_ntt_poly_sub_wrap | ✅ |
| test_ntt_poly_add_sub_inverse | ✅ |
| test_ntt_poly_sub_null | ✅ |

*329 / 329 Unity tests PASS*

### Performance

Not available. `numx_bench` on Windows is built from `benchmarks/bench_win.c`, which does
not call `numx_bench_ntt()` (that call exists only in `benchmarks/bench_runner.c`, used on
the POSIX build path). No separate 32-bit bench target was built for this validation.

**RESULTS: 29 PASS / 0 FAIL / 29 TOTAL**

---

## Windows x64 - Windows 11 / MSVC 19.51.36246.0 (VS 2026 Build Tools) / float64
**Validator:** Amir Ab Khoshk | **Date:** 2026-07-03 | **Commit:** daf5b9c

> Built from the root CMakeLists.txt (which already includes the NTT module) with
> `CMAKE_C_FLAGS="-DNUMX_USE_DOUBLE -DUNITY_INCLUDE_DOUBLE"` passed on the command line,
> so `numx_real_t = double` throughout without editing any existing CMake files.
> Confirmed still a 64-bit x86-64 binary via `file build_x64_f64/Release/numx_tests.exe`.

### Test cases

| Test | Result |
|------|--------|
| test_ntt_forward_delta | ✅ |
| test_ntt_forward_zero | ✅ |
| test_ntt_forward_null | ✅ |
| test_ntt_inverse_roundtrip_delta | ✅ |
| test_ntt_inverse_roundtrip_x | ✅ |
| test_ntt_inverse_roundtrip_x2 | ✅ |
| test_ntt_inverse_roundtrip_full | ✅ |
| test_ntt_inverse_null | ✅ |
| test_ntt_pointwise_mul_identity | ✅ |
| test_ntt_pointwise_mul_known | ✅ |
| test_ntt_pointwise_mul_null | ✅ |
| test_ntt_polymul_delta_identity | ✅ |
| test_ntt_polymul_x_times_x | ✅ |
| test_ntt_polymul_wrap_negacyclic | ✅ |
| test_ntt_polymul_x255_times_x | ✅ |
| test_ntt_polymul_commutativity | ✅ |
| test_ntt_polymul_known_linear | ✅ |
| test_ntt_polymul_random_vs_ref | ✅ |
| test_ntt_polymul_null | ✅ |
| test_ntt_reduce_noop_in_range | ✅ |
| test_ntt_reduce_boundary | ✅ |
| test_ntt_reduce_null | ✅ |
| test_ntt_poly_add_basic | ✅ |
| test_ntt_poly_add_wrap | ✅ |
| test_ntt_poly_add_null | ✅ |
| test_ntt_poly_sub_basic | ✅ |
| test_ntt_poly_sub_wrap | ✅ |
| test_ntt_poly_add_sub_inverse | ✅ |
| test_ntt_poly_sub_null | ✅ |

*329 / 329 Unity tests PASS*

### Performance

Not available. Same `bench_win.c` gap as above; no float64 bench target was built for
this validation.

**RESULTS: 29 PASS / 0 FAIL / 29 TOTAL**

---

## Linux x86-64 - Ubuntu 24.04.4 LTS (WSL2, kernel 6.6.114.1-microsoft-standard-WSL2) / gcc 13.3.0 / float64
**Validator:** Amir Ab Khoshk | **Date:** 2026-07-03 | **Commit:** daf5b9c

> Built from the root CMakeLists.txt with `CMAKE_C_FLAGS="-DNUMX_USE_DOUBLE -DUNITY_INCLUDE_DOUBLE"`
> passed on the command line, no existing CMake files edited. `numx_val_bench_phase2`
> fails to build under float64 (pre-existing float32-only pointer types in that tool), so
> only the `numx_tests` and `numx_bench` targets were built, both of which build cleanly.

### Test cases

| Test | Result |
|------|--------|
| test_ntt_forward_delta | ✅ |
| test_ntt_forward_zero | ✅ |
| test_ntt_forward_null | ✅ |
| test_ntt_inverse_roundtrip_delta | ✅ |
| test_ntt_inverse_roundtrip_x | ✅ |
| test_ntt_inverse_roundtrip_x2 | ✅ |
| test_ntt_inverse_roundtrip_full | ✅ |
| test_ntt_inverse_null | ✅ |
| test_ntt_pointwise_mul_identity | ✅ |
| test_ntt_pointwise_mul_known | ✅ |
| test_ntt_pointwise_mul_null | ✅ |
| test_ntt_polymul_delta_identity | ✅ |
| test_ntt_polymul_x_times_x | ✅ |
| test_ntt_polymul_wrap_negacyclic | ✅ |
| test_ntt_polymul_x255_times_x | ✅ |
| test_ntt_polymul_commutativity | ✅ |
| test_ntt_polymul_known_linear | ✅ |
| test_ntt_polymul_random_vs_ref | ✅ |
| test_ntt_polymul_null | ✅ |
| test_ntt_reduce_noop_in_range | ✅ |
| test_ntt_reduce_boundary | ✅ |
| test_ntt_reduce_null | ✅ |
| test_ntt_poly_add_basic | ✅ |
| test_ntt_poly_add_wrap | ✅ |
| test_ntt_poly_add_null | ✅ |
| test_ntt_poly_sub_basic | ✅ |
| test_ntt_poly_sub_wrap | ✅ |
| test_ntt_poly_add_sub_inverse | ✅ |
| test_ntt_poly_sub_null | ✅ |

*329 / 329 Unity tests PASS*

### Performance

| Function | N | Total | Per call |
|----------|---|-------|----------|
| numx_ntt_forward | 10,000 | 18,980 us | 1,898 ns |
| numx_ntt_inverse | 10,000 | 11,390 us | 1,139 ns |
| numx_ntt_polymul | 10,000 | 53,180 us | 5,318 ns |
| numx_ntt_poly_add | 10,000 | 1,210 us | 121 ns |
| numx_ntt_poly_sub | 10,000 | 1,310 us | 131 ns |

**RESULTS: 29 PASS / 0 FAIL / 29 TOTAL**

---

## Linux x86 - Ubuntu 24.04.4 LTS (WSL2, kernel 6.6.114.1-microsoft-standard-WSL2) / gcc 13.3.0 (-m32, gcc-multilib) / float32
**Validator:** Amir Ab Khoshk | **Date:** 2026-07-03 | **Commit:** daf5b9c

> Configured with `CMAKE_C_FLAGS=-m32 CMAKE_EXE_LINKER_FLAGS=-m32` against the root
> CMakeLists.txt, no existing files edited. Confirmed a 32-bit ELF binary via
> `file build_linux_x86/numx_tests`. Required `gcc-multilib`/`g++-multilib` to be installed
> in WSL first (installed by the user with sudo, not by this session).

### Test cases

| Test | Result |
|------|--------|
| test_ntt_forward_delta | ✅ |
| test_ntt_forward_zero | ✅ |
| test_ntt_forward_null | ✅ |
| test_ntt_inverse_roundtrip_delta | ✅ |
| test_ntt_inverse_roundtrip_x | ✅ |
| test_ntt_inverse_roundtrip_x2 | ✅ |
| test_ntt_inverse_roundtrip_full | ✅ |
| test_ntt_inverse_null | ✅ |
| test_ntt_pointwise_mul_identity | ✅ |
| test_ntt_pointwise_mul_known | ✅ |
| test_ntt_pointwise_mul_null | ✅ |
| test_ntt_polymul_delta_identity | ✅ |
| test_ntt_polymul_x_times_x | ✅ |
| test_ntt_polymul_wrap_negacyclic | ✅ |
| test_ntt_polymul_x255_times_x | ✅ |
| test_ntt_polymul_commutativity | ✅ |
| test_ntt_polymul_known_linear | ✅ |
| test_ntt_polymul_random_vs_ref | ✅ |
| test_ntt_polymul_null | ✅ |
| test_ntt_reduce_noop_in_range | ✅ |
| test_ntt_reduce_boundary | ✅ |
| test_ntt_reduce_null | ✅ |
| test_ntt_poly_add_basic | ✅ |
| test_ntt_poly_add_wrap | ✅ |
| test_ntt_poly_add_null | ✅ |
| test_ntt_poly_sub_basic | ✅ |
| test_ntt_poly_sub_wrap | ✅ |
| test_ntt_poly_add_sub_inverse | ✅ |
| test_ntt_poly_sub_null | ✅ |

*329 / 329 Unity tests PASS*

### Performance

| Function | N | Total | Per call |
|----------|---|-------|----------|
| numx_ntt_forward | 10,000 | 26,250 us | 2,625 ns |
| numx_ntt_inverse | 10,000 | 18,010 us | 1,801 ns |
| numx_ntt_polymul | 10,000 | 76,910 us | 7,691 ns |
| numx_ntt_poly_add | 10,000 | 1,970 us | 197 ns |
| numx_ntt_poly_sub | 10,000 | 2,010 us | 201 ns |

**RESULTS: 29 PASS / 0 FAIL / 29 TOTAL**

---

## ARM64 - Raspbian GNU/Linux 13 (trixie) / Raspberry Pi 4 Model B / gcc 14.2.0 / float32
**Validator:** Amir Ab Khoshk | **Date:** 2026-07-03 | **Commit:** daf5b9c

> Validated over SSH (`sellol@raspberrypi.local`), kernel 6.18.33+rpt-rpi-v8 aarch64.
> Repo was on an older branch, fetched and fast-forwarded to `main` at daf5b9c first.
> Built from the root CMakeLists.txt, unmodified, Release config.

### Test cases

| Test | Result |
|------|--------|
| test_ntt_forward_delta | ✅ |
| test_ntt_forward_zero | ✅ |
| test_ntt_forward_null | ✅ |
| test_ntt_inverse_roundtrip_delta | ✅ |
| test_ntt_inverse_roundtrip_x | ✅ |
| test_ntt_inverse_roundtrip_x2 | ✅ |
| test_ntt_inverse_roundtrip_full | ✅ |
| test_ntt_inverse_null | ✅ |
| test_ntt_pointwise_mul_identity | ✅ |
| test_ntt_pointwise_mul_known | ✅ |
| test_ntt_pointwise_mul_null | ✅ |
| test_ntt_polymul_delta_identity | ✅ |
| test_ntt_polymul_x_times_x | ✅ |
| test_ntt_polymul_wrap_negacyclic | ✅ |
| test_ntt_polymul_x255_times_x | ✅ |
| test_ntt_polymul_commutativity | ✅ |
| test_ntt_polymul_known_linear | ✅ |
| test_ntt_polymul_random_vs_ref | ✅ |
| test_ntt_polymul_null | ✅ |
| test_ntt_reduce_noop_in_range | ✅ |
| test_ntt_reduce_boundary | ✅ |
| test_ntt_reduce_null | ✅ |
| test_ntt_poly_add_basic | ✅ |
| test_ntt_poly_add_wrap | ✅ |
| test_ntt_poly_add_null | ✅ |
| test_ntt_poly_sub_basic | ✅ |
| test_ntt_poly_sub_wrap | ✅ |
| test_ntt_poly_add_sub_inverse | ✅ |
| test_ntt_poly_sub_null | ✅ |

*329 / 329 Unity tests PASS*

### Performance

| Function | N | Total | Per call |
|----------|---|-------|----------|
| numx_ntt_forward | 10,000 | 265,890 us | 26,589 ns |
| numx_ntt_inverse | 10,000 | 219,660 us | 21,966 ns |
| numx_ntt_polymul | 10,000 | 813,320 us | 81,332 ns |
| numx_ntt_poly_add | 10,000 | 24,100 us | 2,410 ns |
| numx_ntt_poly_sub | 10,000 | 27,370 us | 2,737 ns |

**RESULTS: 29 PASS / 0 FAIL / 29 TOTAL**

---

## ESP32-S3 - pending

Not yet validated in this session, no direct access to the ESP32-S3 device was available.
