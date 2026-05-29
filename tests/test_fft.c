/**
 * @file test_fft.c
 * @brief Tests for numx FFT functions.
 *
 * Complex data layout: [re0, im0, re1, im1, ...]
 * Twiddle-factor tolerance is loose (1e-3f) because priv_cos/sin uses
 * a Taylor approximation rather than a hardware FPU instruction.
 */

#include "unity.h"
#include "numx/fft.h"

#define TOL      1e-3f   /* float32 + Taylor-approx twiddle tolerance */
#define TOL_TIGHT 1e-4f  /* for exact-integer results (DC, magnitude) */

/* ════════════════════════════════════════════════════════════════════
 *  numx_fft_f32
 * ════════════════════════════════════════════════════════════════════ */

/* L1 */
void test_fft_dc_n4(void)
{
    /* Input: 4 samples of 1+0i.  DFT: X[0]=4, X[1..3]=0 */
    numx_real_t x[] = {1.0f,0.0f, 1.0f,0.0f, 1.0f,0.0f, 1.0f,0.0f};
    TEST_ASSERT_EQUAL(NUMX_OK, numx_fft_f32(x, 4));
    TEST_ASSERT_FLOAT_WITHIN(TOL_TIGHT, 4.0f, x[0]);   /* re X[0] */
    TEST_ASSERT_FLOAT_WITHIN(TOL_TIGHT, 0.0f, x[1]);   /* im X[0] */
    TEST_ASSERT_FLOAT_WITHIN(TOL_TIGHT, 0.0f, x[2]);   /* re X[1] */
    TEST_ASSERT_FLOAT_WITHIN(TOL_TIGHT, 0.0f, x[4]);   /* re X[2] */
    TEST_ASSERT_FLOAT_WITHIN(TOL_TIGHT, 0.0f, x[6]);   /* re X[3] */
}

void test_fft_single_tone_n4(void)
{
    /* Input: e^{2*pi*i*k/4} for k=0..3 = [1+0i, 0+1i, -1+0i, 0-1i]
     * DFT: X[0]=0, X[1]=4, X[2]=0, X[3]=0 */
    numx_real_t x[] = {1.0f,0.0f, 0.0f,1.0f, -1.0f,0.0f, 0.0f,-1.0f};
    TEST_ASSERT_EQUAL(NUMX_OK, numx_fft_f32(x, 4));
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, x[0]);   /* re X[0] */
    TEST_ASSERT_FLOAT_WITHIN(TOL, 4.0f, x[2]);   /* re X[1] */
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, x[3]);   /* im X[1] */
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, x[4]);   /* re X[2] */
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, x[6]);   /* re X[3] */
}

void test_fft_dc_n2(void)
{
    /* 2-point DFT of [1,1]: X[0]=2, X[1]=0 */
    numx_real_t x[] = {1.0f,0.0f, 1.0f,0.0f};
    TEST_ASSERT_EQUAL(NUMX_OK, numx_fft_f32(x, 2));
    TEST_ASSERT_FLOAT_WITHIN(TOL_TIGHT, 2.0f, x[0]);
    TEST_ASSERT_FLOAT_WITHIN(TOL_TIGHT, 0.0f, x[1]);
    TEST_ASSERT_FLOAT_WITHIN(TOL_TIGHT, 0.0f, x[2]);
    TEST_ASSERT_FLOAT_WITHIN(TOL_TIGHT, 0.0f, x[3]);
}

/* L2 */
void test_fft_n1_passthrough(void)
{
    /* 1-point DFT: output equals input */
    numx_real_t x[] = {3.5f, -1.2f};
    TEST_ASSERT_EQUAL(NUMX_OK, numx_fft_f32(x, 1));
    TEST_ASSERT_FLOAT_WITHIN(TOL_TIGHT, 3.5f,  x[0]);
    TEST_ASSERT_FLOAT_WITHIN(TOL_TIGHT, -1.2f, x[1]);
}

void test_fft_delta_at_0(void)
{
    /* Input: [N, 0, 0, 0] (delta scaled by N).  DFT: [N, N, N, N] */
    numx_real_t x[] = {4.0f,0.0f, 0.0f,0.0f, 0.0f,0.0f, 0.0f,0.0f};
    TEST_ASSERT_EQUAL(NUMX_OK, numx_fft_f32(x, 4));
    TEST_ASSERT_FLOAT_WITHIN(TOL_TIGHT, 4.0f, x[0]);
    TEST_ASSERT_FLOAT_WITHIN(TOL_TIGHT, 4.0f, x[2]);
    TEST_ASSERT_FLOAT_WITHIN(TOL_TIGHT, 4.0f, x[4]);
    TEST_ASSERT_FLOAT_WITHIN(TOL_TIGHT, 4.0f, x[6]);
}

/* L4 */
void test_fft_null_returns_error(void)
{
    TEST_ASSERT_EQUAL(NUMX_ERR_NULL_PTR, numx_fft_f32(NULL, 4));
}

void test_fft_n_not_power_of_two(void)
{
    numx_real_t x[6] = {0};
    TEST_ASSERT_EQUAL(NUMX_ERR_INVALID_ARG, numx_fft_f32(x, 3));
}

void test_fft_n_exceeds_max(void)
{
    numx_real_t x[2] = {0};
    TEST_ASSERT_EQUAL(NUMX_ERR_INVALID_ARG, numx_fft_f32(x, NUMX_MAX_FFT_SIZE * 2));
}

/* ════════════════════════════════════════════════════════════════════
 *  numx_ifft_f32
 * ════════════════════════════════════════════════════════════════════ */

/* L1 */
void test_ifft_roundtrip(void)
{
    /* IFFT(FFT(x)) == x for an arbitrary real signal */
    numx_real_t orig[] = {1.0f,0.0f, 2.0f,0.0f, 3.0f,0.0f, 0.0f,0.0f};
    numx_real_t x[]    = {1.0f,0.0f, 2.0f,0.0f, 3.0f,0.0f, 0.0f,0.0f};
    numx_size_t i;
    TEST_ASSERT_EQUAL(NUMX_OK, numx_fft_f32(x, 4));
    TEST_ASSERT_EQUAL(NUMX_OK, numx_ifft_f32(x, 4));
    for (i = 0; i < 8; i++)
        TEST_ASSERT_FLOAT_WITHIN(TOL, orig[i], x[i]);
}

void test_ifft_dc_spectrum(void)
{
    /* IFFT([4,0, 0,0, 0,0, 0,0]) = [1,0, 1,0, 1,0, 1,0] */
    numx_real_t x[] = {4.0f,0.0f, 0.0f,0.0f, 0.0f,0.0f, 0.0f,0.0f};
    TEST_ASSERT_EQUAL(NUMX_OK, numx_ifft_f32(x, 4));
    TEST_ASSERT_FLOAT_WITHIN(TOL, 1.0f, x[0]);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, x[1]);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 1.0f, x[2]);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 1.0f, x[4]);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 1.0f, x[6]);
}

/* L4 */
void test_ifft_null_returns_error(void)
{
    TEST_ASSERT_EQUAL(NUMX_ERR_NULL_PTR, numx_ifft_f32(NULL, 4));
}

/* ════════════════════════════════════════════════════════════════════
 *  numx_fft_q15
 * ════════════════════════════════════════════════════════════════════ */

/* L1 */
void test_fft_q15_dc_n4(void)
{
    /* Input: 4 samples of 0x4000 (≈0.5 in Q15), all real.
     * With per-stage right-shift, DC bin = 0x4000 (scaled by 1/n). */
    numx_q15_t x[] = {0x4000,0, 0x4000,0, 0x4000,0, 0x4000,0};
    TEST_ASSERT_EQUAL(NUMX_OK, numx_fft_q15(x, 4));
    /* DC bin real part: expect ~0x4000, allow ±2 LSB for rounding */
    TEST_ASSERT_INT16_WITHIN(2, 0x4000, x[0]);
    TEST_ASSERT_INT16_WITHIN(2, 0,      x[1]);
    /* All other bins near zero */
    TEST_ASSERT_INT16_WITHIN(2, 0, x[2]);
    TEST_ASSERT_INT16_WITHIN(2, 0, x[4]);
    TEST_ASSERT_INT16_WITHIN(2, 0, x[6]);
}

/* L4 */
void test_fft_q15_null_returns_error(void)
{
    TEST_ASSERT_EQUAL(NUMX_ERR_NULL_PTR, numx_fft_q15(NULL, 4));
}

void test_fft_q15_n_not_power_of_two(void)
{
    numx_q15_t x[6] = {0};
    TEST_ASSERT_EQUAL(NUMX_ERR_INVALID_ARG, numx_fft_q15(x, 3));
}

/* ════════════════════════════════════════════════════════════════════
 *  numx_fft_magnitude
 * ════════════════════════════════════════════════════════════════════ */

/* L1 */
void test_fft_magnitude_dc(void)
{
    /* After DC FFT of N=4 all-ones: X[0]=4+0i, rest zero.
     * Magnitude[0]=4, [1]=0, [2]=0. */
    numx_real_t fft_out[] = {4.0f,0.0f, 0.0f,0.0f, 0.0f,0.0f, 0.0f,0.0f};
    numx_real_t mag[3];
    TEST_ASSERT_EQUAL(NUMX_OK, numx_fft_magnitude(fft_out, 4, mag));
    TEST_ASSERT_FLOAT_WITHIN(TOL_TIGHT, 4.0f, mag[0]);
    TEST_ASSERT_FLOAT_WITHIN(TOL_TIGHT, 0.0f, mag[1]);
    TEST_ASSERT_FLOAT_WITHIN(TOL_TIGHT, 0.0f, mag[2]);
}

void test_fft_magnitude_complex_bin(void)
{
    /* Bin with re=3, im=4: magnitude = 5 */
    numx_real_t fft_out[] = {3.0f,4.0f, 0.0f,0.0f, 0.0f,0.0f};
    numx_real_t mag[2];
    TEST_ASSERT_EQUAL(NUMX_OK, numx_fft_magnitude(fft_out, 2, mag));
    TEST_ASSERT_FLOAT_WITHIN(TOL, 5.0f, mag[0]);
}

/* L4 */
void test_fft_magnitude_null_returns_error(void)
{
    numx_real_t fft_out[4] = {0}, mag[3] = {0};
    TEST_ASSERT_EQUAL(NUMX_ERR_NULL_PTR, numx_fft_magnitude(NULL, 4, mag));
    TEST_ASSERT_EQUAL(NUMX_ERR_NULL_PTR, numx_fft_magnitude(fft_out, 4, NULL));
}

void test_fft_magnitude_n1_invalid(void)
{
    numx_real_t fft_out[2] = {0}, mag[1] = {0};
    TEST_ASSERT_EQUAL(NUMX_ERR_INVALID_ARG, numx_fft_magnitude(fft_out, 1, mag));
}

/* ════════════════════════════════════════════════════════════════════
 *  Suite entry point
 * ════════════════════════════════════════════════════════════════════ */

void numx_test_fft(void)
{
    RUN_TEST(test_fft_dc_n4);
    RUN_TEST(test_fft_single_tone_n4);
    RUN_TEST(test_fft_dc_n2);
    RUN_TEST(test_fft_n1_passthrough);
    RUN_TEST(test_fft_delta_at_0);
    RUN_TEST(test_fft_null_returns_error);
    RUN_TEST(test_fft_n_not_power_of_two);
    RUN_TEST(test_fft_n_exceeds_max);
    RUN_TEST(test_ifft_roundtrip);
    RUN_TEST(test_ifft_dc_spectrum);
    RUN_TEST(test_ifft_null_returns_error);
    RUN_TEST(test_fft_q15_dc_n4);
    RUN_TEST(test_fft_q15_null_returns_error);
    RUN_TEST(test_fft_q15_n_not_power_of_two);
    RUN_TEST(test_fft_magnitude_dc);
    RUN_TEST(test_fft_magnitude_complex_bin);
    RUN_TEST(test_fft_magnitude_null_returns_error);
    RUN_TEST(test_fft_magnitude_n1_invalid);
}
