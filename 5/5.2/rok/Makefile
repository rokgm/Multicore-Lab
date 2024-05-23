all: reduction 

reduction : reduction.c
	mpicc -o reduction reduction.c 

clean:
	rm -f *.o reduction *~

remake : clean all

