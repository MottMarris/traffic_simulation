gcc -ansi -I./ -c queue.c -o queue.o
gcc -ansi -I./ -c sim.c -o sim.o
gcc sim.o queue.o -lgsl -lgslcblas -o sim
./sim
