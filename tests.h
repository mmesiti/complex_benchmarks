#include "basic_macros.h"
#include "common.h"

#define _prod_c99_complex_op(x, y, z, ASSIGNOP, repeats, m)                    \
  _MLOOP(m, i) z[i] = 0;                                                       \
  _REPEATLOOP(elapsed, c, repeats, m, i) z[i] ASSIGNOP x[i] * y[i];            \
  _ENDREPEATLOOP(elapsed);                                                     \
  BenchmarkResult res = {.elapsed = elapsed, .cc = c};                         \
  return res

BenchmarkResult prod_c99_complex_add(const REAL complex *x,
                                     const REAL complex *y, REAL complex *z,
                                     int repeats, int m) {
  _prod_c99_complex_op(x, y, z, +=, repeats, m);
}

BenchmarkResult prod_c99_complex_eq(const REAL complex *x,
                                    const REAL complex *y, REAL complex *z,
                                    int repeats, int m) {
  _prod_c99_complex_op(x, y, z, =, repeats, m);
}

#define _prod_c99_complex_v2_op(x, y, z, ASSIGNOP, repeats, m)                 \
  _MLOOP(m, i) z[i] = 0;                                                       \
  _REPEATLOOP(elapsed, c, repeats, m, i)                                       \
  z[i] ASSIGNOP creal(x[i]) * creal(y[i]) - cimag(x[i]) * cimag(y[i]) +        \
      I *(creal(x[i]) * cimag(y[i]) + cimag(x[i]) * creal(y[i]));              \
  _ENDREPEATLOOP(elapsed);                                                     \
  BenchmarkResult res = {.elapsed = elapsed, .cc = c};                         \
  return res

BenchmarkResult prod_c99_complex_v2_eq(const REAL complex *x,
                                       const REAL complex *y, REAL complex *z,
                                       int repeats, int m) {
  _prod_c99_complex_v2_op(x, y, z, =, repeats, m);
}

BenchmarkResult prod_c99_complex_v2_add(const REAL complex *x,
                                        const REAL complex *y, REAL complex *z,
                                        int repeats, int m) {
  _prod_c99_complex_v2_op(x, y, z, +=, repeats, m);
}

#define _prod_c99_complex_v3_op(x, y, z, ASSIGNOP, repeats, m)                 \
  REAL *Z = (REAL *)z;                                                         \
  REAL *X = (REAL *)x;                                                         \
  REAL *Y = (REAL *)y;                                                         \
  _MLOOP(2 * m, i) z[i] = 0;                                                   \
  _REPEATLOOP(elapsed, c, repeats, m, i)                                       \
  Z[2 * i] ASSIGNOP X[2 * i] * Y[2 * i] - X[2 * i + 1] * Y[2 * i + 1];         \
  Z[2 * i + 1] ASSIGNOP X[2 * i + 1] * Y[2 * i] + X[2 * i] * Y[2 * i + 1];     \
  _ENDREPEATLOOP(elapsed);                                                     \
  BenchmarkResult res = {.elapsed = elapsed, .cc = c};                         \
  return res

BenchmarkResult prod_c99_complex_v3_add(const REAL complex *x,
                                        const REAL complex *y, REAL complex *z,
                                        int repeats, int m) {
  _prod_c99_complex_v3_op(x, y, z, +=, repeats, m);
}
BenchmarkResult prod_c99_complex_v3_eq(const REAL complex *x,
                                       const REAL complex *y, REAL complex *z,
                                       int repeats, int m) {
  _prod_c99_complex_v3_op(x, y, z, =, repeats, m);
}

#define prod_split_complex_op(xre, xim, yre, yim, zre, zim, ASSIGNOP, repeats, \
                              m)                                               \
  _MLOOP(2 * m, i) zre[i] = 0;                                                 \
  _MLOOP(2 * m, i) zim[i] = 0;                                                 \
  _REPEATLOOP(elapsed, c, repeats, m, i)                                       \
  zre[i] ASSIGNOP xre[i] * yre[i] - xim[i] * yim[i];                           \
  zim[i] ASSIGNOP xre[i] * yim[i] + xim[i] * yre[i];                           \
  _ENDREPEATLOOP(elapsed);                                                     \
  BenchmarkResult res = {.elapsed = elapsed, .cc = c};                         \
  return res

BenchmarkResult prod_split_complex_eq(const REAL *xre, const REAL *xim,
                                      const REAL *yre, const REAL *yim,
                                      REAL *zre, REAL *zim, int repeats,
                                      int m) {
  prod_split_complex_op(xre, xim, yre, yim, zre, zim, =, repeats, m);
}

BenchmarkResult prod_split_complex_add(const REAL *xre, const REAL *xim,
                                       const REAL *yre, const REAL *yim,
                                       REAL *zre, REAL *zim, int repeats,
                                       int m) {
  prod_split_complex_op(xre, xim, yre, yim, zre, zim, +=, repeats, m);
}

#define prod_split_complex_v2_op(xre, xim, yre, yim, zre, zim, ASSIGNOP,       \
                                 repeat, m)                                    \
  _MLOOP(m, i) zre[i] = 0;                                                     \
  _MLOOP(m, i) zim[i] = 0;                                                     \
  register int c = 0;                                                          \
  struct timeval tstart, tend;                                                 \
  gettimeofday(&tstart, NULL);                                                 \
  for (int _ = 0; _ < repeats; ++_) {                                          \
    _MLOOP(m, i) {                                                             \
      c++;                                                                     \
      zre[i] ASSIGNOP xre[i] * yre[i] - xim[i] * yim[i];                       \
    }                                                                          \
    _MLOOP(m, i) {                                                             \
      c++;                                                                     \
      zim[i] ASSIGNOP xre[i] * yim[i] + xim[i] * yre[i];                       \
    }                                                                          \
  }                                                                            \
  gettimeofday(&tend, NULL);                                                   \
  BenchmarkResult res = {.elapsed = tdiff(tstart, tend), .cc = c};             \
  return res

BenchmarkResult prod_split_complex_v2_add(const REAL *xre, const REAL *xim,
                                          const REAL *yre, const REAL *yim,
                                          REAL *zre, REAL *zim, int repeats,
                                          int m) {

  prod_split_complex_v2_op(xre, xim, yre, yim, zre, zim, +=, repeat, m);
}

BenchmarkResult prod_split_complex_v2_eq(const REAL *xre, const REAL *xim,
                                         const REAL *yre, const REAL *yim,
                                         REAL *zre, REAL *zim, int repeats,
                                         int m) {

  prod_split_complex_v2_op(xre, xim, yre, yim, zre, zim, =, repeat, m);
}
