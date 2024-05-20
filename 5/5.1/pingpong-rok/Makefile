all: pingpong 

pingpong : pingpong.c
	mpicc -o pingpong pingpong.c -O3 -xhost

clean:
	rm -f *.o pingpong *~

remake : clean all

