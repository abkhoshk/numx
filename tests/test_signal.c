/**
 * @file test_signal.c
 * @brief Tests for numx signal processing functions.
 */

#include "unity.h"
#include "numx/signal.h"

#define TOL 1e-4f

/* ════════════════════════════════════════════════════════════════════
 *  Window — rectangular
 * ════════════════════════════════════════════════════════════════════ */

/* L1 */
void test_window_rect_all_ones(void)
{
    numx_real_t w[8];
    numx_size_t i;
    TEST_ASSERT_EQUAL(NUMX_OK, numx_signal_window_rect(8, w));
    for (i = 0; i < 8; i++)
        TEST_ASSERT_FLOAT_WITHIN(TOL, 1.0f, w[i]);
}

/* L3 */
void test_window_rect_n1(void)
{
    numx_real_t w[1];
    TEST_ASSERT_EQUAL(NUMX_OK, numx_signal_window_rect(1, w));
    TEST_ASSERT_FLOAT_WITHIN(TOL, 1.0f, w[0]);
}

/* ════════════════════════════════════════════════════════════════════
 *  Window — Hann
 * ════════════════════════════════════════════════════════════════════ */

/* L1 */
void test_window_hann_endpoints_zero(void)
{
    numx_real_t w[5];
    TEST_ASSERT_EQUAL(NUMX_OK, numx_signal_window_hann(5, w));
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, w[0]);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, w[4]);
}

void test_window_hann_midpoint_one(void)
{
    /* n=5, i=2: 0.5*(1 - cos(pi)) = 1.0 */
    numx_real_t w[5];
    TEST_ASSERT_EQUAL(NUMX_OK, numx_signal_window_hann(5, w));
    TEST_ASSERT_FLOAT_WITHIN(TOL, 1.0f, w[2]);
}

/* L3 */
void test_window_hann_n1_returns_one(void)
{
    numx_real_t w[1];
    TEST_ASSERT_EQUAL(NUMX_OK, numx_signal_window_hann(1, w));
    TEST_ASSERT_FLOAT_WITHIN(TOL, 1.0f, w[0]);
}

/* ════════════════════════════════════════════════════════════════════
 *  Window — Hamming
 * ════════════════════════════════════════════════════════════════════ */

/* L1 */
void test_window_hamming_endpoints(void)
{
    /* w[0] = w[n-1] = 0.54 - 0.46 = 0.08 */
    numx_real_t w[5];
    TEST_ASSERT_EQUAL(NUMX_OK, numx_signal_window_hamming(5, w));
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.08f, w[0]);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.08f, w[4]);
}

void test_window_hamming_midpoint_one(void)
{
    /* n=5, i=2: 0.54 - 0.46*cos(pi) = 1.0 */
    numx_real_t w[5];
    TEST_ASSERT_EQUAL(NUMX_OK, numx_signal_window_hamming(5, w));
    TEST_ASSERT_FLOAT_WITHIN(TOL, 1.0f, w[2]);
}

/* ════════════════════════════════════════════════════════════════════
 *  Window — Blackman
 * ════════════════════════════════════════════════════════════════════ */

/* L1 */
void test_window_blackman_endpoints_zero(void)
{
    /* w[0] = 0.42 - 0.5 + 0.08 = 0.0 */
    numx_real_t w[5];
    TEST_ASSERT_EQUAL(NUMX_OK, numx_signal_window_blackman(5, w));
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, w[0]);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, w[4]);
}

void test_window_blackman_midpoint_one(void)
{
    /* n=5, i=2: 0.42 + 0.5 + 0.08 = 1.0 */
    numx_real_t w[5];
    TEST_ASSERT_EQUAL(NUMX_OK, numx_signal_window_blackman(5, w));
    TEST_ASSERT_FLOAT_WITHIN(TOL, 1.0f, w[2]);
}

/* ════════════════════════════════════════════════════════════════════
 *  Convolution
 * ════════════════════════════════════════════════════════════════════ */

/* L1 */
void test_convolve_box(void)
{
    /* [1,1,1] * [1,1,1] = [1,2,3,2,1] */
    numx_real_t x[] = {1.0f, 1.0f, 1.0f};
    numx_real_t h[] = {1.0f, 1.0f, 1.0f};
    numx_real_t out[5];
    TEST_ASSERT_EQUAL(NUMX_OK, numx_signal_convolve(x, 3, h, 3, out));
    TEST_ASSERT_FLOAT_WITHIN(TOL, 1.0f, out[0]);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 2.0f, out[1]);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 3.0f, out[2]);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 2.0f, out[3]);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 1.0f, out[4]);
}

void test_convolve_unit_impulse(void)
{
    /* convolve with delta: output should equal the other signal */
    numx_real_t x[]   = {1.0f, 0.0f, 0.0f};
    numx_real_t h[]   = {3.0f, 2.0f, 1.0f};
    numx_real_t out[5];
    TEST_ASSERT_EQUAL(NUMX_OK, numx_signal_convolve(x, 3, h, 3, out));
    TEST_ASSERT_FLOAT_WITHIN(TOL, 3.0f, out[0]);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 2.0f, out[1]);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 1.0f, out[2]);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, out[3]);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, out[4]);
}

/* L4 */
void test_convolve_null_returns_error(void)
{
    numx_real_t x[2] = {0}, h[2] = {0}, out[3] = {0};
    TEST_ASSERT_EQUAL(NUMX_ERR_NULL_PTR, numx_signal_convolve(NULL, 2, h, 2, out));
    TEST_ASSERT_EQUAL(NUMX_ERR_NULL_PTR, numx_signal_convolve(x, 2, NULL, 2, out));
    TEST_ASSERT_EQUAL(NUMX_ERR_NULL_PTR, numx_signal_convolve(x, 2, h, 2, NULL));
}

/* ════════════════════════════════════════════════════════════════════
 *  Cross-correlation
 * ════════════════════════════════════════════════════════════════════ */

/* L1 */
void test_correlate_autocorr_peak_at_lag0(void)
{
    /* autocorrelation of [1,1,1]: peak (=3) at lag 0, which is out[yn-1]=out[2] */
    numx_real_t x[] = {1.0f, 1.0f, 1.0f};
    numx_real_t out[5];
    TEST_ASSERT_EQUAL(NUMX_OK, numx_signal_correlate(x, 3, x, 3, out));
    /* lag-0 value = sum x[j]*x[j] = 3 */
    TEST_ASSERT_FLOAT_WITHIN(TOL, 3.0f, out[2]);
    /* lag+1 and lag-1 = 2 */
    TEST_ASSERT_FLOAT_WITHIN(TOL, 2.0f, out[1]);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 2.0f, out[3]);
}

/* L2 */
void test_correlate_output_length(void)
{
    /* output has xn+yn-1 elements; check boundary values are well-defined */
    numx_real_t x[] = {1.0f, 2.0f};
    numx_real_t y[] = {1.0f, 0.0f, 0.0f};
    numx_real_t out[4];
    TEST_ASSERT_EQUAL(NUMX_OK, numx_signal_correlate(x, 2, y, 3, out));
    /* lag = k-(yn-1): k=0 → lag=-2, k=3 → lag=1 */
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f, out[0]); /* no overlap at lag -2 */
}

/* ════════════════════════════════════════════════════════════════════
 *  FIR filter
 * ════════════════════════════════════════════════════════════════════ */

/* L1 */
void test_fir_identity_tap(void)
{
    numx_real_t x[]    = {1.0f, 2.0f, 3.0f};
    numx_real_t taps[] = {1.0f};
    numx_real_t out[3];
    TEST_ASSERT_EQUAL(NUMX_OK, numx_signal_fir(x, 3, taps, 1, out));
    TEST_ASSERT_FLOAT_WITHIN(TOL, 1.0f, out[0]);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 2.0f, out[1]);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 3.0f, out[2]);
}

void test_fir_moving_average(void)
{
    /* 3-tap moving average on DC signal: ramps up, then reaches steady state */
    numx_real_t x[]    = {3.0f, 3.0f, 3.0f, 3.0f, 3.0f};
    numx_real_t third  = 1.0f / 3.0f;
    numx_real_t taps[] = {third, third, third};
    numx_real_t out[5];
    TEST_ASSERT_EQUAL(NUMX_OK, numx_signal_fir(x, 5, taps, 3, out));
    TEST_ASSERT_FLOAT_WITHIN(TOL, 1.0f, out[0]); /* only x[0] contributes */
    TEST_ASSERT_FLOAT_WITHIN(TOL, 2.0f, out[1]);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 3.0f, out[2]);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 3.0f, out[3]);
}

/* L4 */
void test_fir_null_returns_error(void)
{
    numx_real_t x[2] = {0}, taps[1] = {1.0f}, out[2] = {0};
    TEST_ASSERT_EQUAL(NUMX_ERR_NULL_PTR, numx_signal_fir(NULL, 2, taps, 1, out));
    TEST_ASSERT_EQUAL(NUMX_ERR_NULL_PTR, numx_signal_fir(x, 2, NULL, 1, out));
    TEST_ASSERT_EQUAL(NUMX_ERR_NULL_PTR, numx_signal_fir(x, 2, taps, 1, NULL));
}

/* ════════════════════════════════════════════════════════════════════
 *  IIR biquad
 * ════════════════════════════════════════════════════════════════════ */

/* L1 */
void test_iir_biquad_identity(void)
{
    /* b=[1,0,0], a=[0,0] → pass-through */
    numx_real_t x[] = {1.0f, 2.0f, 3.0f};
    numx_real_t b[] = {1.0f, 0.0f, 0.0f};
    numx_real_t a[] = {0.0f, 0.0f};
    numx_real_t out[3];
    TEST_ASSERT_EQUAL(NUMX_OK, numx_signal_iir_biquad(x, 3, b, a, out));
    TEST_ASSERT_FLOAT_WITHIN(TOL, 1.0f, out[0]);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 2.0f, out[1]);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 3.0f, out[2]);
}

void test_iir_biquad_scale(void)
{
    /* b=[2,0,0], a=[0,0] → scale by 2 */
    numx_real_t x[] = {1.0f, 2.0f, 3.0f};
    numx_real_t b[] = {2.0f, 0.0f, 0.0f};
    numx_real_t a[] = {0.0f, 0.0f};
    numx_real_t out[3];
    TEST_ASSERT_EQUAL(NUMX_OK, numx_signal_iir_biquad(x, 3, b, a, out));
    TEST_ASSERT_FLOAT_WITHIN(TOL, 2.0f, out[0]);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 4.0f, out[1]);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 6.0f, out[2]);
}

/* L4 */
void test_iir_biquad_null_returns_error(void)
{
    numx_real_t x[2] = {0}, b[] = {1, 0, 0}, a[] = {0, 0}, out[2] = {0};
    TEST_ASSERT_EQUAL(NUMX_ERR_NULL_PTR, numx_signal_iir_biquad(NULL, 2, b, a, out));
    TEST_ASSERT_EQUAL(NUMX_ERR_NULL_PTR, numx_signal_iir_biquad(x, 2, NULL, a, out));
    TEST_ASSERT_EQUAL(NUMX_ERR_NULL_PTR, numx_signal_iir_biquad(x, 2, b, NULL, out));
    TEST_ASSERT_EQUAL(NUMX_ERR_NULL_PTR, numx_signal_iir_biquad(x, 2, b, a, NULL));
}

/* ════════════════════════════════════════════════════════════════════
 *  Peak detection
 * ════════════════════════════════════════════════════════════════════ */

/* L1 */
void test_peaks_two_peaks(void)
{
    numx_real_t x[]  = {0.0f, 1.0f, 0.0f, 2.0f, 0.0f};
    numx_size_t peaks[5];
    numx_size_t npeaks;
    TEST_ASSERT_EQUAL(NUMX_OK, numx_signal_peaks(x, 5, peaks, 5, &npeaks));
    TEST_ASSERT_EQUAL(2, npeaks);
    TEST_ASSERT_EQUAL(1, peaks[0]);
    TEST_ASSERT_EQUAL(3, peaks[1]);
}

/* L2 */
void test_peaks_monotone_no_peaks(void)
{
    numx_real_t x[]  = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f};
    numx_size_t peaks[5];
    numx_size_t npeaks;
    TEST_ASSERT_EQUAL(NUMX_OK, numx_signal_peaks(x, 5, peaks, 5, &npeaks));
    TEST_ASSERT_EQUAL(0, npeaks);
}

/* L3 */
void test_peaks_short_signal_no_peaks(void)
{
    numx_real_t x[]  = {5.0f, 5.0f};
    numx_size_t peaks[2];
    numx_size_t npeaks;
    TEST_ASSERT_EQUAL(NUMX_OK, numx_signal_peaks(x, 2, peaks, 2, &npeaks));
    TEST_ASSERT_EQUAL(0, npeaks);
}

/* L4 */
void test_peaks_buffer_too_small(void)
{
    numx_real_t x[]  = {0.0f, 1.0f, 0.0f, 2.0f, 0.0f};
    numx_size_t peaks[1];
    numx_size_t npeaks;
    TEST_ASSERT_EQUAL(NUMX_ERR_BUFFER_SMALL, numx_signal_peaks(x, 5, peaks, 1, &npeaks));
}

/* ════════════════════════════════════════════════════════════════════
 *  EMA
 * ════════════════════════════════════════════════════════════════════ */

/* L1 */
void test_ema_alpha1_copies_input(void)
{
    numx_real_t x[]  = {1.0f, 2.0f, 3.0f};
    numx_real_t out[3];
    TEST_ASSERT_EQUAL(NUMX_OK, numx_signal_ema(x, 3, 1.0f, out));
    TEST_ASSERT_FLOAT_WITHIN(TOL, 1.0f, out[0]);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 2.0f, out[1]);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 3.0f, out[2]);
}

void test_ema_alpha0_stays_at_first(void)
{
    numx_real_t x[]  = {5.0f, 1.0f, 1.0f, 1.0f};
    numx_real_t out[4];
    TEST_ASSERT_EQUAL(NUMX_OK, numx_signal_ema(x, 4, 0.0f, out));
    TEST_ASSERT_FLOAT_WITHIN(TOL, 5.0f, out[0]);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 5.0f, out[1]);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 5.0f, out[2]);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 5.0f, out[3]);
}

/* L2 */
void test_ema_known_sequence(void)
{
    /* x=[0,1,0,0], alpha=0.5: out=[0, 0.5, 0.25, 0.125] */
    numx_real_t x[]  = {0.0f, 1.0f, 0.0f, 0.0f};
    numx_real_t out[4];
    TEST_ASSERT_EQUAL(NUMX_OK, numx_signal_ema(x, 4, 0.5f, out));
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.0f,   out[0]);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.5f,   out[1]);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.25f,  out[2]);
    TEST_ASSERT_FLOAT_WITHIN(TOL, 0.125f, out[3]);
}

/* L4 */
void test_ema_invalid_alpha_returns_error(void)
{
    numx_real_t x[2] = {0}, out[2];
    TEST_ASSERT_EQUAL(NUMX_ERR_INVALID_ARG, numx_signal_ema(x, 2, -0.1f, out));
    TEST_ASSERT_EQUAL(NUMX_ERR_INVALID_ARG, numx_signal_ema(x, 2,  1.1f, out));
}

/* ════════════════════════════════════════════════════════════════════
 *  Suite entry point
 * ════════════════════════════════════════════════════════════════════ */

void numx_test_signal(void)
{
    RUN_TEST(test_window_rect_all_ones);
    RUN_TEST(test_window_rect_n1);
    RUN_TEST(test_window_hann_endpoints_zero);
    RUN_TEST(test_window_hann_midpoint_one);
    RUN_TEST(test_window_hann_n1_returns_one);
    RUN_TEST(test_window_hamming_endpoints);
    RUN_TEST(test_window_hamming_midpoint_one);
    RUN_TEST(test_window_blackman_endpoints_zero);
    RUN_TEST(test_window_blackman_midpoint_one);
    RUN_TEST(test_convolve_box);
    RUN_TEST(test_convolve_unit_impulse);
    RUN_TEST(test_convolve_null_returns_error);
    RUN_TEST(test_correlate_autocorr_peak_at_lag0);
    RUN_TEST(test_correlate_output_length);
    RUN_TEST(test_fir_identity_tap);
    RUN_TEST(test_fir_moving_average);
    RUN_TEST(test_fir_null_returns_error);
    RUN_TEST(test_iir_biquad_identity);
    RUN_TEST(test_iir_biquad_scale);
    RUN_TEST(test_iir_biquad_null_returns_error);
    RUN_TEST(test_peaks_two_peaks);
    RUN_TEST(test_peaks_monotone_no_peaks);
    RUN_TEST(test_peaks_short_signal_no_peaks);
    RUN_TEST(test_peaks_buffer_too_small);
    RUN_TEST(test_ema_alpha1_copies_input);
    RUN_TEST(test_ema_alpha0_stays_at_first);
    RUN_TEST(test_ema_known_sequence);
    RUN_TEST(test_ema_invalid_alpha_returns_error);
}
