# Intel compiler
CC =  icc
CFLAGS = -O2
LIKWID_FLAGS = $(LIKWID_INC) $(LIKWID_LIB)

MPICC = mpicc

all: heat

heat : heat.o input.o misc.o timing.o relax_gauss.o relax_jacobi.o
	$(CC) $(CFLAGS) $(LIKWID_FLAGS) -o $@ $+ -lm -llikwid

%.o : %.c heat.h timing.h input.h
	$(CC) $(CFLAGS) $(LIKWID_FLAGS) -c -o $@ $< -DLIKWID_PERFMON

clean:
	rm -f *.o heat *~ *.ppm

remake : clean all
