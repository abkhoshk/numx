# Fast Fourier Transform

**Module:** `numx/fft.h` | **Category:** Signal Processing | **Phase:** P2.08–P2.10

---

## What it solves

Computes the Discrete Fourier Transform (DFT) and its inverse in $O(n \log n)$ time using the Cooley-Tukey radix-2 algorithm. Two variants are provided: a float32 FFT/IFFT for general use, and a scaled Q1.15 fixed-point FFT for integer DSP pipelines (e.g. microcontrollers without FPU). A magnitude helper extracts the single-sided amplitude spectrum.

---

## Mathematical formulations

### Discrete Fourier Transform

For a complex sequence $x[0], \ldots, x[n-1]$:

$$X[k] = \sum_{j=0}^{n-1} x[j]\, e^{-2\pi i\, jk/n}, \qquad k = 0, \ldots, n-1$$

The inverse DFT recovers $x$ from $X$:

$$x[j] = \frac{1}{n} \sum_{k=0}^{n-1} X[k]\, e^{2\pi i\, jk/n}$$

### Cooley-Tukey radix-2 DIT

Recursively splits the DFT into two half-length transforms (Decimation-In-Time). For $n = 2^s$:

1. **Bit-reversal permutation** — reorders samples so the recursion can proceed in-place:

$$x'[i] \leftrightarrow x'[\text{bitrev}(i)]$$

2. **Butterfly stages** — for each stage $s = 1, \ldots, \log_2 n$, with $m = 2^s$, $W_m = e^{-2\pi i/m}$:

$$X[u] \leftarrow X[u] + W_m^j \cdot X[v]$$
$$X[v] \leftarrow X[u] - W_m^j \cdot X[v]$$

where $u = k + j$, $v = k + j + m/2$, and $W_m^j = \cos(2\pi j/m) - i\sin(2\pi j/m)$ is the **twiddle factor**.

Total stages: $\log_2 n$. Function evaluations of cos/sin: $n/2 \cdot \log_2 n$.

### IFFT via conjugation

The IFFT is computed without a separate algorithm:

1. Conjugate input: $X[k] \leftarrow \overline{X[k]}$
2. Apply forward FFT
3. Conjugate output and scale: $x[j] \leftarrow \overline{X[j]} / n$

### Twiddle factors

Twiddle factors are computed on-the-fly using the library's internal Taylor-series cosine ($12$ terms, $< 1\,\text{ULP}$ error on $[0, \pi]$). No lookup table is stored, saving RAM at the cost of ~$12$ multiplies per factor.

$$W_m^j : \quad \text{re} = \cos\!\left(\frac{\pi j}{m/2}\right), \quad \text{im} = -\sin\!\left(\frac{\pi j}{m/2}\right)$$

### Q15 scaled FFT

For integer pipelines, inputs and twiddle factors are Q1.15 values ($[-1, 1)$ mapped to $[-32768, 32767]$). To prevent overflow at each butterfly, both outputs are right-shifted by 1:

$$X[u] \leftarrow (X[u] + W \cdot X[v]) \gg 1$$
$$X[v] \leftarrow (X[u] - W \cdot X[v]) \gg 1$$

After $\log_2 n$ stages the output is **scaled by $1/n$** relative to the true DFT. Q15 multiply with rounding:

$$\text{q15\_mul}(a, b) = \left\lfloor \frac{a \cdot b + 2^{14}}{2^{15}} \right\rfloor$$

### Magnitude spectrum

Single-sided amplitude spectrum for bins $k = 0, \ldots, n/2$:

$$|X[k]| = \sqrt{\text{Re}(X[k])^2 + \text{Im}(X[k])^2}$$

Produces $n/2 + 1$ values (DC to Nyquist). Bins $k > n/2$ are the conjugate mirror of $k < n/2$ for real-valued inputs.

---

## Complexity

| Function | Time | Stack |
|---|---|---|
| `numx_fft_f32` | $O(n \log n)$ | negligible (in-place) |
| `numx_ifft_f32` | $O(n \log n)$ | negligible |
| `numx_fft_q15` | $O(n \log n)$ | negligible (in-place) |
| `numx_fft_magnitude` | $O(n)$ | negligible |

Maximum transform size: `NUMX_MAX_FFT_SIZE` (default 512).

---

## Data format

All transforms use **interleaved complex** layout:

```
inout = [ re0, im0, re1, im1, ..., re_{n-1}, im_{n-1} ]
```

Buffer length is `2*n` elements. For real-valued signals, set all imaginary parts to zero before calling `numx_fft_f32`.

---

## When to use

- **`numx_fft_f32`**: spectral analysis, frequency-domain filtering, convolution of long signals ($n > 256$), sensor data characterisation.
- **`numx_ifft_f32`**: frequency-domain synthesis, deconvolution, channel equalisation.
- **`numx_fft_q15`**: integer DSP pipelines on MCUs without FPU (e.g. ARM Cortex-M0); note the $1/n$ output scaling.
- **`numx_fft_magnitude`**: computing amplitude spectrum for display or peak-frequency detection.

## When NOT to use

- `n` not a power of two — returns `NUMX_ERR_INVALID_ARG`; zero-pad to the next power of two.
- `n > NUMX_MAX_FFT_SIZE` — increase the config limit (affects no stack, just the validation bound).
- Q15 FFT on signals that require the absolute DFT amplitude — the $1/n$ scaling must be compensated by the caller.
- For very small $n$ (2 or 4) where the FFT overhead is not worth it — direct DFT is simpler.

---

## References

1. Cooley, J. W. & Tukey, J. W. — "An algorithm for the machine computation of complex Fourier series", *Mathematics of Computation* 19(90):297–301, 1965.
2. Oppenheim, A. V. & Schafer, R. W. — *Discrete-Time Signal Processing*, 3rd ed., §9, Prentice Hall, 2009.
3. Proakis, J. G. & Manolakis, D. G. — *Digital Signal Processing*, 4th ed., §8.1, Pearson, 2006.

---

## Example

```c
#include "numx/fft.h"
#include "numx/signal.h"

/* 8-point FFT of a real-valued signal */
numx_real_t samples[8] = {1, 2, 3, 4, 3, 2, 1, 0};
numx_real_t buf[16];   /* 2*n interleaved */
numx_real_t mag[5];    /* n/2 + 1 = 5 bins */

for (int k = 0; k < 8; k++) { buf[2*k] = samples[k]; buf[2*k+1] = 0.0f; }

/* Apply Hann window before FFT */
numx_real_t win[8];
numx_signal_window_hann(8, win);
for (int k = 0; k < 8; k++) buf[2*k] *= win[k];

numx_fft_f32(buf, 8);
numx_fft_magnitude(buf, 8, mag);
/* mag[0] = DC, mag[1..4] = harmonic bins up to Nyquist */

/* Q15 example: 4-point FFT of DC input (output scaled by 1/4) */
numx_q15_t qbuf[] = {0x4000,0, 0x4000,0, 0x4000,0, 0x4000,0};
numx_fft_q15(qbuf, 4);
/* qbuf[0] ≈ 0x4000 (DC bin, scaled), rest ≈ 0 */
```
