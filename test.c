#include <stdio.h>
#include "aesrng.h"
#include "mt.h"
#include "cpuid.h"

int main(void) {
  if (!intel_has_feature(INTEL_FEATURE_AES)) {
    printf("No Intel AES instruction support detected on this processor\n");
    return -1;
  }
  
  char buffer[1 << 10] __attribute__ ((aligned (16)));
  mt_state_t state;
  mt_init(state, 1234);
  aesrng_seed(12345);
  srand48(123);

  while (1) {
    aesrng_fill(buffer, 1 << 10);
    /*mt_fill(state, buffer, 1<<10);*/ /* mersenne twister, for comparison */
    write(STDOUT_FILENO, buffer, 1 << 10);
  }
}
