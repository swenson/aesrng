# aesrng

A simple AES-based random number generator for modern x86-64 processors in C.

## Usage

It's as easy as including `aesrng.h`, seeding with `aesrng_fill(12345)` (or something),
and calling, e.g., `aesrng_get_double()`.

```c
#include <stdio.h>
#include "aesrng.h"
#include "cpuid.h"

int main(void) {
  if (!intel_has_feature(INTEL_FEATURE_AES)) {
    printf("No Intel AES instruction support detected on this processor\n");
    return -1;
  }

  char buffer[1 << 10] __attribute__ ((aligned (16)));
  aesrng_seed(12345);

  /* generate some random numbers to an array */
  aesrng_fill(buffer, 1 << 10);

  /* or get a double, etc. */
  aesrng_get_double();

  /* or get a 64-bit int, etc. */
  aesrng_get_uint64();
}
```

## See also

[This blog post](http://caswenson.com) about the library.

## Caveats

* The random number generator is *probably* not thread-safe, although it
  may be. It could compile down to a single instruction if inlined just right,
  but I wouldn't count on it.
* The random numbers are pretty good, but they aren't as good as those from, e.g.,
  the Mersenne Twister.
* This is a bit faster than the Mersenne Twister.
* You can trade off speed for random quality by increasing the number of rounds
  used.

## License

All code in this repository, unless otherwise specified, is hereby
licensed under the MIT Public License:

Copyright (c) 2015 Christopher Swenson.

	Permission is hereby granted, free of charge, to any person
	obtaining a copy of this software and associated documentation
	files (the "Software"), to deal in the Software without
	restriction, including without limitation the rights to use,
	copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the
	Software is furnished to do so, subject to the following
	conditions:

	The above copyright notice and this permission notice shall be
	included in all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
	OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
	NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
	HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
	WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
	FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
	OTHER DEALINGS IN THE SOFTWARE.
