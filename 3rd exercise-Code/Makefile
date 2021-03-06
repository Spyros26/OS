#Makefile
#

CC = gcc

# CAUTION: Always use '-pthread' when compiling POSIX threads-based
# applications, instead of linking with "-lpthread" directly.
CFLAGS = -Wall -O2 -pthread
LIBS =

all: pthread-test simplesync-mutex simplesync-atomic mandel original_mandel 
## Pthread test
pthread-test: pthread-test.o
        $(CC) $(CFLAGS) -o pthread-test pthread-test.o $(LIBS)

pthread-test.o: pthread-test.c
        $(CC) $(CFLAGS) -c -o pthread-test.o pthread-test.c

## Simple sync (two versions)
simplesync-mutex: simplesync-mutex.o
        $(CC) $(CFLAGS) -o simplesync-mutex simplesync-mutex.o $(LIBS)

simplesync-atomic: simplesync-atomic.o
        $(CC) $(CFLAGS) -o simplesync-atomic simplesync-atomic.o $(LIBS)

simplesync-mutex.o: simplesync.c
        $(CC) $(CFLAGS) -DSYNC_MUTEX -c -o simplesync-mutex.o simplesync.c

simplesync-atomic.s: simplesync.c
        $(CC) -DSYNC_ATOMIC simplesync.c -S -g

simplesync-atomic.o: simplesync.c
        $(CC) $(CFLAGS) -DSYNC_ATOMIC -c -o simplesync-atomic.o simplesync.c


## Mandel
mandel: mandel-lib.o mandel.o
        $(CC) $(CFLAGS) -o mandel mandel-lib.o mandel.o $(LIBS)

mandel-lib.o: mandel-lib.h mandel-lib.c
        $(CC) $(CFLAGS) -c -o mandel-lib.o mandel-lib.c $(LIBS)

mandel.o: mandel.c
        $(CC) $(CFLAGS) -c -o mandel.o mandel.c $(LIBS)


##Original_ Mandel
original_mandel: mandel-lib.o original_mandel.o
        $(CC) $(CFLAGS) -o original_mandel mandel-lib.o original_mandel.o $(LIBS)

original_mandel.o: original_mandel.c
        $(CC) $(CFLAGS) -c -o original_mandel.o original_mandel.c $(LIBS)


clean:
        rm -f s *.o pthread-test simplesync-{atomic,mutex} mandel original_mandel