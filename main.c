#define _POSIX_C_SOURCE 200212L

#include <complex.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

//#if (_POSIX_C_SOURCE >= 200112L || _XOPEN_SOURCE >= 600)
#if (_POSIX_C_SOURCE < 200112L && _XOPEN_SOURCE < 600)
#error "Posix Memalign Not Defined"
#endif

#include "tests.h"

int main(int argc, char **argv) {

  int n = 1 << 24;
  int m = 1;
  double elapsed_time;
  REAL complex *x, *y, *z;
  REAL *xre, *yre, *zre;
  REAL *xim, *yim, *zim;

  // INITIALISATION
  posix_memalign((void **)&x, ALIGN, n * sizeof(REAL complex));
  posix_memalign((void **)&y, ALIGN, n * sizeof(REAL complex));
  posix_memalign((void **)&z, ALIGN, n * sizeof(REAL complex));

  posix_memalign((void **)&xre, ALIGN, n * sizeof(REAL));
  posix_memalign((void **)&yre, ALIGN, n * sizeof(REAL));
  posix_memalign((void **)&zre, ALIGN, n * sizeof(REAL));

  posix_memalign((void **)&xim, ALIGN, n * sizeof(REAL));
  posix_memalign((void **)&yim, ALIGN, n * sizeof(REAL));
  posix_memalign((void **)&zim, ALIGN, n * sizeof(REAL));

  srand(42);

  for (int i = 0; i < n; ++i) {
    x[i] = myrandom() + myrandom() * I;
    y[i] = myrandom() + myrandom() * I;
    xre[i] = creal(x[i]);
    yre[i] = creal(y[i]);
    xim[i] = cimag(x[i]);
    yim[i] = cimag(y[i]);
  }

  printf("mode size repeats total_time\n");
  for (double mm = 1; mm < n; mm *= 1.2) {
    int m = mm;
    const int repeats = (n / m > 1000 ? 1000 : n / m);
    int cc;
#define _run_test_c99(FUN, tag)                                                \
  {                                                                            \
    BenchmarkResult tmp = FUN(x, y, z, repeats, m);                            \
    elapsed_time = tmp.elapsed;                                                \
    cc = tmp.cc;                                                               \
  }                                                                            \
  {                                                                            \
    REAL complex s = sum(z, m);                                                \
    fprintf(stderr, tag "[Size: %d, repeats: %d] *, sum : %f, %f, count %d\n", \
            m, repeats, creal(s), cimag(s), cc);                               \
    printf(tag " %d %d %f\n", m, repeats, elapsed_time);                       \
  }

    _run_test_c99(prod_c99_complex_add, "com1+=");
    _run_test_c99(prod_c99_complex_eq, "com1= ");

    _run_test_c99(prod_c99_complex_v2_add, "com2+=");
    _run_test_c99(prod_c99_complex_v2_eq, "com2= ");

    _run_test_c99(prod_c99_complex_v3_add, "com3+=");
    _run_test_c99(prod_c99_complex_v3_eq, "com3= ");

#define _run_test_split(FUN, tag)                                              \
  {                                                                            \
    BenchmarkResult tmp = FUN(xre, xim, yre, yim, zre, zim, repeats, m);       \
    elapsed_time = tmp.elapsed;                                                \
    cc = tmp.cc;                                                               \
  }                                                                            \
  {                                                                            \
    REAL sre = resum(zre, m);                                                  \
    REAL sim = resum(zim, m);                                                  \
    fprintf(stderr, tag "[Size: %d, repeats: %d] *, sum : %f, %f count %d\n",  \
            m, repeats, sre, sim, cc);                                         \
    printf(tag " %d %d %f\n", m, repeats, elapsed_time);                       \
  }

    _run_test_split(prod_split_complex_add, "split1+=");
    _run_test_split(prod_split_complex_eq, "split1= ");

    _run_test_split(prod_split_complex_v2_add, "split2+=");
    _run_test_split(prod_split_complex_v2_eq, "split2= ");
  }
  return 0;
}
