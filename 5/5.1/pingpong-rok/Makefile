all: pingpong 

pingpong : pingpong.c
	mpicc -o pingpong pingpong.c 

clean:
	rm -f *.o pingpong *~

remake : clean all

