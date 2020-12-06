#ifndef __MACROS_H_
#define __MACROS_H_

#define _MLOOP(m, i) for (int i = 0; i < m; ++i)

#define _REPEATLOOP(elapsed, c, repeats, m, i)                                 \
  double elapsed;                                                              \
  register int c = 0;                                                          \
  {                                                                            \
    struct timeval tstart, tend;                                               \
    gettimeofday(&tstart, NULL);                                               \
    for (int _ = 0; _ < repeats; ++_)                                          \
      _MLOOP(m, i) {                                                           \
        c++;

#define _ENDREPEATLOOP(elapsed)                                                \
  }                                                                            \
  gettimeofday(&tend, NULL);                                                   \
  elapsed = tdiff(tstart, tend);                                               \
  }



#endif // __MACROS_H_
