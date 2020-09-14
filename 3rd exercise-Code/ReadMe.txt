3.1
This program uses two threads. The first increase the value of variable val and the second decreases it.
It produces an executable where the two threads are syncronized with mutexes and another where synchronization is achieved with atomic operations.

To see the assembly code run:

gcc -DSYNC_ATOMIC simplesync.c -S -g
or gcc -DSYNC_MUTEX simplesync.c -S -g

(a file named simplesync.s is produced in the current directory)

3.2
A program to draw the Mandelbrot Set on a 256-color xterm, using threads.

The Makefile produces all the executeables for the 3rd exercise.