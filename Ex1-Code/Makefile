all: zing zing2 fconc pro3

zing: zing.o main.o
        gcc -o zing zing.o main.o

zing2: zing2.o main.o
        gcc -o zing2 zing2.o main.o

fconc: main2.o
        gcc -o fconc main2.o

pro3: profconc.o
        gcc -o pro3 profconc.o

profconc.o: profconc.c
        gcc -Wall -c profconc.c

main2.o: main2.c
        gcc -Wall -c main2.c

zing2.o: zing2.c
        gcc -Wall -c zing2.c

main.o: main.c
        gcc -Wall -c main.c
