#ifndef __AES_RNG_H__
#define __AES_RNG_H__



#if __x86_64__

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <wmmintrin.h>

void aesrng_seed(int seed);
double aesrng_get_double();
int32_t aesrng_get_int32();
uint32_t aesrng_get_uint32();
int64_t aesrng_get_int64();
uint64_t aesrng_get_uint64();
void aesrng_fill(void *data, size_t size);


#define AES_ROUNDS 1

static volatile __m128i aesrng_accum;
static volatile __m128i aesrng_key;
static int aesrng_seeded = 0;

void aesrng_seed(int seed) {
  aesrng_accum = _mm_setr_epi32(seed, seed * 2, seed * 3, seed * 4);
  aesrng_key = _mm_setr_epi32(seed * 5, seed * 6, seed * 7, seed * 8);
  aesrng_seeded = 1;
}

static __inline__ __m128i aesrng() {
  static int once = 1;

  if (once) {
    if (!aesrng_seeded) {
      aesrng_seed(1234);
    }

    once = 0;
  }

  __m128i temp = aesrng_accum;
  int i;

  for (i = 0; i < AES_ROUNDS; i++) {
    temp = _mm_aesenc_si128(temp, aesrng_key);
  }

  aesrng_accum = temp;
  return temp;
}

double aesrng_get_double() {
  __m128i temp = aesrng();
  uint64_t num = (uint64_t) _mm_cvtsi128_si64(temp);
  return (double) num / 18446744073709551615.0;
}

int32_t aesrng_get_int32() {
  __m128i temp = aesrng();
  return (int32_t) _mm_cvtsi128_si32(temp);
}

uint32_t aesrng_get_uint32() {
  __m128i temp = aesrng();
  return (uint32_t) _mm_cvtsi128_si32(temp);
}

int64_t aesrng_get_int64() {
  __m128i temp = aesrng();
  return (int32_t) _mm_cvtsi128_si64(temp);
}

uint64_t aesrng_get_uint64() {
  __m128i temp = aesrng();
  return (uint64_t) _mm_cvtsi128_si64(temp);
}

void aesrng_fill(void *data, size_t size) {
  long addr = (long) data;
  __m128i *data128;

  /* check for initial alignment off */
  if ((addr & 0xf) != 0) {
    char *chardata = (char *) data;
    int todo = 0xf - (addr & 0xf) + 1;
    todo = (todo > size) ? size : todo;
    char temp[todo] __attribute__ ((aligned (16)));
    __m128i *temp128 = (__m128i *) temp;
    __m128i t = aesrng();
    _mm_store_si128(temp128, t);
    memcpy(chardata, temp, todo);
    data128 = (__m128i *) ((addr | 0xf) + 1);
    size -= todo;
  } else {
    data128 = (__m128i *) data;
  }

  for (int i = 0; i < size >> 4; i++) {
    __m128i temp = aesrng();
    _mm_store_si128(&data128[i], temp);
  }

  int left = size & 0xf;

  if (left != 0) {
    char *chardata = (char *) &data128[(size >> 4) + 1];
    char temp[left] __attribute__ ((aligned (16)));
    __m128i *temp128 = (__m128i *) temp;
    __m128i t = aesrng();
    _mm_store_si128(temp128, t);
    memcpy(chardata, temp, left);
  }

  return;
}

#endif


#endif
