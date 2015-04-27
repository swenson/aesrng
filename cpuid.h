#if __x86_64__

#include <wmmintrin.h>

#define cpuid(f, ax, bx,cx,dx) __asm__ __volatile__ ("cpuid": "=a" (ax), "=b" (bx), "=c" (cx), "=d" (dx) : "a" (f));

#define INTEL_FEATURE_AES ((int[3]) {0, 1<<25, 0})

int intel_has_feature(int *features);

int intel_has_feature(int *features) {
  int ax, bx, cx, dx;
  int present = -1;
  cpuid(1, ax, bx, cx, dx);

  if (features[0] != 0) {
    present = (features[0] & dx) && present;
  }

  if (features[1] != 0) {
    present = (features[1] & cx) && present;
  }

  if (features[2] != 0) {
    present = (features[2] & bx) && present;
  }

  return present != 0;
}

#endif
