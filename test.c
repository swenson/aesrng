#include <stdio.h>
#include "aesrng.h"
#include "mt.h"

int main(void) {
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
