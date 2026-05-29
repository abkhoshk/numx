# Signal Processing

**Module:** `numx/signal.h` | **Category:** Signal Processing | **Phase:** P2.01–P2.07

---

## What it provides

A collection of discrete-time signal processing primitives that require no dynamic allocation: window functions for spectral analysis, linear convolution, cross-correlation, causal FIR and IIR (biquad) filtering, local peak detection, and exponential moving average.

---

## Mathematical formulations

### Window functions

All windows produce $n$ weights $w[i]$ for $i = 0, \ldots, n-1$, applied element-wise to a signal before an FFT to reduce spectral leakage.

| Window | Formula | Sidelobe level |
|---|---|---|
| Rectangular | $w[i] = 1$ | $-13\,\text{dB}$ |
| Hann | $w[i] = \dfrac{1}{2}\!\left(1 - \cos\!\dfrac{2\pi i}{n-1}\right)$ | $-31\,\text{dB}$ |
| Hamming | $w[i] = 0.54 - 0.46\cos\!\dfrac{2\pi i}{n-1}$ | $-41\,\text{dB}$ |
| Blackman | $w[i] = 0.42 - 0.5\cos\!\dfrac{2\pi i}{n-1} + 0.08\cos\!\dfrac{4\pi i}{n-1}$ | $-57\,\text{dB}$ |

Hann and Blackman endpoints are identically zero (no discontinuity). Hamming endpoints are $0.08$ (a small but non-zero pedestal). The rectangular window has the narrowest main lobe but the highest sidelobes — always prefer Hann or Blackman for spectral analysis.

### Linear convolution

For signals $x[0..x_n\!-\!1]$ and $h[0..h_n\!-\!1]$:

$$\text{out}[k] = \sum_{j=\max(0,\,k-h_n+1)}^{\min(x_n-1,\,k)} x[j]\, h[k - j], \qquad k = 0, \ldots, x_n + h_n - 2$$

Output length: $x_n + h_n - 1$. Complexity: $O(x_n \cdot h_n)$.

### Cross-correlation

$$R_{xy}[k] = \sum_{j} x[j]\, y[j - (k - (y_n - 1))], \qquad k = 0, \ldots, x_n + y_n - 2$$

Lag $0$ is at index $y_n - 1$; negative lags at lower indices, positive lags at higher. Equivalent to convolving $x$ with $y$ time-reversed.

### FIR filter

Causal finite impulse response filter with $M$ taps, zero initial conditions:

$$\text{out}[n] = \sum_{k=0}^{M-1} h[k]\, x[n - k], \qquad x[n] = 0 \text{ for } n < 0$$

Direct-form implementation. Complexity: $O(x_n \cdot M)$. Maximum taps: `NUMX_MAX_FIR_TAPS`.

### IIR biquad filter

Second-order infinite impulse response filter, Direct Form II:

$$d_0[n] = x[n] - a_1 d_1[n] - a_2 d_2[n]$$
$$y[n]   = b_0 d_0[n] + b_1 d_1[n] + b_2 d_2[n]$$
$$d_2[n+1] = d_1[n], \quad d_1[n+1] = d_0[n]$$

Transfer function:

$$H(z) = \frac{b_0 + b_1 z^{-1} + b_2 z^{-2}}{1 + a_1 z^{-1} + a_2 z^{-2}}$$

The $a_0 = 1$ normalisation is assumed; the caller passes $a = \{a_1, a_2\}$.

### Peak detection

A sample $x[i]$ is a **strict local maximum** if:

$$x[i] > x[i-1] \quad \text{and} \quad x[i] > x[i+1], \qquad 1 \leq i \leq n-2$$

Endpoints are never classified as peaks.

### Exponential moving average (EMA)

$$\text{out}[0] = x[0], \qquad \text{out}[i] = \alpha\, x[i] + (1 - \alpha)\,\text{out}[i-1], \quad \alpha \in [0, 1]$$

$\alpha = 1$: no smoothing (output equals input). $\alpha \to 0$: heavy smoothing (output stays near $x[0]$). The time constant in samples is $\tau \approx 1/\alpha$.

---

## Complexity

| Function | Time | Notes |
|---|---|---|
| `numx_signal_window_*` | $O(n)$ | One Taylor-series cos per sample |
| `numx_signal_convolve` | $O(x_n \cdot h_n)$ | Direct sum; use FFT-based convolution for large signals |
| `numx_signal_correlate` | $O(x_n \cdot y_n)$ | Same kernel as convolution |
| `numx_signal_fir` | $O(x_n \cdot M)$ | Direct form; no state between calls |
| `numx_signal_iir_biquad` | $O(n)$ | 5 multiply-add per sample |
| `numx_signal_peaks` | $O(n)$ | Single pass |
| `numx_signal_ema` | $O(n)$ | 2 multiply-add per sample |

---

## When to use

- **Windows**: apply before FFT to reduce spectral leakage; choose Blackman for high dynamic-range measurements.
- **Convolve / correlate**: signal alignment, matched filtering, template detection. For lengths > 256, prefer an FFT-based approach.
- **FIR**: linear-phase filtering (FIR guarantees zero phase distortion); coefficients designed offline (e.g. Parks-McClellan).
- **IIR biquad**: low-latency, low-pass / high-pass / notch filters in real-time loops; cascade multiple biquads for higher-order filters.
- **Peaks**: peak-picking after spectral analysis, threshold-based event detection.
- **EMA**: sensor fusion, DC bias estimation, low-latency smoothing in control loops.

## When NOT to use

- `numx_signal_convolve` / `correlate` for large signals (> 512 samples) — $O(n^2)$ cost; prefer FFT convolution.
- `numx_signal_fir` with `ntaps > NUMX_MAX_FIR_TAPS` — returns `NUMX_ERR_INVALID_ARG`; increase the config limit.
- `numx_signal_iir_biquad` without checking stability ($|a_1|, |a_2|$ must satisfy the stability triangle) — an unstable biquad will overflow.
- `numx_signal_ema` with `alpha` outside $[0, 1]$ — returns `NUMX_ERR_INVALID_ARG`.

---

## References

1. Harris, F. J. — "On the use of windows for harmonic analysis with the discrete Fourier transform", *Proceedings of the IEEE* 66(1):51–83, 1978.
2. Oppenheim, A. V. & Schafer, R. W. — *Discrete-Time Signal Processing*, 3rd ed., Prentice Hall, 2009.
3. Rabiner, L. R. & Gold, B. — *Theory and Application of Digital Signal Processing*, Prentice Hall, 1975.

---

## Example

```c
#include "numx/signal.h"

/* Apply a Hann window then compute EMA of a 64-sample signal */
numx_real_t sig[64];   /* assume filled */
numx_real_t win[64];
numx_real_t smoothed[64];

numx_signal_window_hann(64, win);
for (int i = 0; i < 64; i++) sig[i] *= win[i];

numx_signal_ema(sig, 64, 0.2f, smoothed);

/* 3-tap moving-average FIR */
numx_real_t taps[] = {1.0f/3, 1.0f/3, 1.0f/3};
numx_real_t fir_out[64];
numx_signal_fir(sig, 64, taps, 3, fir_out);
```
