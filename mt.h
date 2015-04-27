/* simple 32-bit implementation of the mersenne twister */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct mt_state_s {
  uint32_t MT[624];
  int index;
} mt_state_s;

typedef mt_state_s mt_state_t[1];

void mt_init(mt_state_t state, int seed);
uint32_t mt_get_uint32(mt_state_t state);
uint64_t mt_get_uint64(mt_state_t state);
double mt_get_double(mt_state_t state);
float mt_get_float(mt_state_t state);

void mt_init(mt_state_t state, int seed) {
  state->MT[0] = seed;
  state->index = 0;
  int i;

  for (i = 1; i < 624; i++) {
    uint32_t temp = (1812433253 * (state->MT[i - 1] ^ (state->MT[i - 1] >> 30))) + i;
    state->MT[i] = temp & 0xffffffff;
  }
}

void mt_generate_numbers(mt_state_t state) {
  int i;
  int i1 = 1;
  int i397 = 397;

  for (i = 0; i < 624; i++) {
    uint32_t y = (state->MT[i] & 0x80000000) + (state->MT[i1] & 0x7fffffff);
    state->MT[i] = state->MT[i397] ^ (y >> 1);

    if ((y & 1) == 1) {
      state->MT[i] ^= 2567483615u;
    }

    i1++;
    i397++;

    if (i1 == 624) { i1 = 0; }

    if (i397 == 624) { i397 = 0; }
  }
}

uint64_t mt_get_uint64(mt_state_t state) {
  uint64_t x = mt_get_uint32(state);
  uint64_t y = mt_get_uint32(state);
  return (x << 32) | y;
}

double mt_get_double(mt_state_t state) {
  return (double) mt_get_uint64(state) / 18446744073709551615.0;
}

float mt_get_float(mt_state_t state) {
  return (float) mt_get_uint32(state) / 4294967295.0f;
}

uint32_t mt_get_uint32(mt_state_t state) {
  if (state->index == 0) {
    mt_generate_numbers(state);
  }

  uint32_t y = state->MT[state->index++];
  y ^= y >> 11;
  y ^= (y << 7) & 2636928640u;
  y ^= (y << 15) & 4022730752u;
  y ^= y >> 18;

  if (state->index == 624) { state->index = 0; }

  return y;
}

void mt_fill(mt_state_t state, void *data, size_t size) {
  uint32_t *data32 = (uint32_t *) data;

  for (int i = 0; i < size >> 2; i++) {
    data32[i] = mt_get_uint32(state);
  }

  int left = size & 3;

  if (left != 0) {
    uint32_t r = mt_get_uint32(state);
    char *chardata = (char *) &data32[(size >> 2) + 1];
    int i = 0;

    switch (left) {
    case 3:
      chardata[i++] = r & 0xff;
      r >>= 8;

    case 2:
      chardata[i++] = r & 0xff;
      r >>= 8;

    case 1:
      chardata[i] = r & 0xff;
    }
  }
}
