#ifndef STRING_H
#define STRING_H

#include <stddef.h>

static inline void* memset(void* s, int c, size_t n) {
  return __builtin_memset(s, c, n);
}

#endif /* STRING_H */
