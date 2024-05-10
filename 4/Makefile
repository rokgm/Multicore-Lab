CC =  icc
CFLAGS = -O3 -fno-alias -xhost -qopt-report5 -qopt-report-phase=par -parallel -par-threshold0 -par-num-threads=4


all: heat 

heat : heat.o input.o misc.o timing.o relax_jacobi.o
	$(CC) $(CFLAGS) -o heat $+ -lm  

%.o : %.c %.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o heat *~ *.ppm

remake : clean all
