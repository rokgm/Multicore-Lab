# Intel compiler
CC =  icc

CFLAGS = -O3 -fno-alias -xhost

MPICC = mpicc

all: heat

heat : heat.o input.o misc.o timing.o relax_gauss.o relax_jacobi.o
	$(CC) $(CFLAGS) -o $@ $+ -lm
# $(LIKWID_LIB) -llikwid 

%.o : %.c heat.h timing.h input.h
	$(CC) $(CFLAGS) -c -o $@ $<
# $(LIKWID_INC) -DLIKWID_PERFMON

clean:
	rm -f *.o heat *~ *.ppm

remake : clean all
