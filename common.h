#ifndef __COMMON_H_
#define __COMMON_H_
#include <complex.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define ALIGN 128
#define REAL float

REAL myrandom() { return (REAL)rand() / RAND_MAX; }

REAL complex sum(REAL complex *z, int m) {
  do {
    for (int i = 0; i < m / 2; ++i) {
      z[i] += z[i + m / 2];
    }
    m = m / 2 + m % 2;
  } while (m > 1);
  return z[0];
}

REAL resum(REAL *z, int m) {
  do {
    for (int i = 0; i < m / 2; ++i) {
      z[i] += z[i + m / 2];
    }
    m = m / 2 + m % 2;
  } while (m > 1);
  return z[0];
}

typedef struct BenchmarkResult {
  double elapsed;
  int cc;
} BenchmarkResult;

double tdiff(struct timeval tstart, struct timeval tend) {
  return (double)(tend.tv_sec - tstart.tv_sec) +
         (double)1e-6 * (tend.tv_usec - tstart.tv_usec);
}

#endif // __COMMON_H_
