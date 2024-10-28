#include "string/strncmp.h"

#include <stddef.h>

int strncmp(const char* lhs, const char* rhs, size_t n) {
  while (n > 0 && *lhs != '\0' && *lhs == *rhs) {
    lhs++;
    rhs++;
    n--;
  }
  return n == 0 ? 0 : (*(unsigned char*)lhs - *(unsigned char*)rhs);
}