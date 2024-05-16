MPICC = mpicc

all: pingpong 

pingpong : pingpong.c
	${MPICC} -o pingpong pingpong.c 

clean:
	rm -f *.o pingpong *~ *.ppm

remake : clean all

