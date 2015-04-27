CC ?= gcc
CFLAGS = -O3 -g -Wall -std=c89 -maes

default: test

.PHONY: default clean test format speedtest

speedtest:
	$(CC) $(CFLAGS) test.c -o test
	./test | pv > /dev/null

test:
	$(CC) $(CFLAGS) test.c -o test
	./test | dieharder -a -g 200

clean:
	rm -f test

format:
	astyle --options=astyle.options *.h *.c

