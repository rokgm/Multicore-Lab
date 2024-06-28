CC =  mpicc
CFLAGS = -O3 -fno-alias -xhost -qopenmp

ifeq ($(VARIANT),NON_BLOCKING)
    JACOBI=relax_jacobi_nonblocking.o
else ifeq ($(VARIANT),HYBRID) 
	CFLAGS += -DHYBRID
	JACOBI=relax_jacobi_hybrid.o
else
	JACOBI=relax_jacobi_blocking.o
endif

# Clean incase varaint is switched.
all: clean heat 

heat : heat.o input.o misc.o timing.o ${JACOBI}
	$(CC) $(CFLAGS) -o heat $+ -lm  

%.o : %.c %.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o heat *~ *.ppm

remake : clean all
