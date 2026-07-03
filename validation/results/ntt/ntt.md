# Validation: numx_ntt

Covers: `numx_ntt_forward` · `numx_ntt_inverse` · `numx_ntt_pointwise_mul` · `numx_ntt_polymul` · `numx_ntt_poly_add` · `numx_ntt_poly_sub` · `numx_ntt_reduce`

---

## Windows x64 - Windows 11 / MSVC 19.51.36246.0 (VS 2026 Build Tools)
**Validator:** Amir Ab Khoshk | **Date:** 2026-07-03 | **Commit:** daf5b9c

> Platform note: this run was performed on Windows/MSVC, not Linux x86-64. Raspberry Pi 4
> and ESP32-S3 validation were not performed in this session, no direct access to that
> hardware was available. This section covers only the Windows x64 platform.

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

Not available for this platform. `numx_bench` on Windows is built from `benchmarks/bench_win.c`,
which does not call `numx_bench_ntt()` (that call exists only in `benchmarks/bench_runner.c`,
used on the POSIX build path). No NTT performance numbers were produced by this run, and
`bench_win.c` was not modified to add it, since only `ntt.md` was in scope for this validation.

**RESULTS: 29 PASS / 0 FAIL / 29 TOTAL**

---

## Linux x86-64 - pending

Not yet validated in this session, no Linux x86-64 machine access was available.

---

## ARM64 - Raspberry Pi 4 - pending

Not yet validated in this session, no direct access to the Raspberry Pi 4 was available.

---

## ESP32-S3 - pending

Not yet validated in this session, no direct access to the ESP32-S3 device was available.
