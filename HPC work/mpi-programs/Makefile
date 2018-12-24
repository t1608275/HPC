all: mpi01 mpi02 mpi03 mpi04 mpi05 mpi06 mpi07 mpi08

mpi01: mpi01.c
	mpicc -o mpi01 mpi01.c

mpi02: mpi02.c
	mpicc -o mpi02 mpi02.c 

mpi03: mpi03.c
	mpicc -o mpi03 mpi03.c

mpi04: mpi04.c
	mpicc -o mpi04 mpi04.c

mpi05: mpi05.c
	mpicc -o mpi05 mpi05.c -lrt

mpi06: mpi06.c
	mpicc -o mpi06 mpi06.c

mpi07: mpi07.c
	mpicc -o mpi07 mpi07.c -lrt

mpi08: mpi08.c
	mpicc -o mpi08 mpi08.c

clean:
	rm -f a.out *~ mpi01 mpi02 mpi03 mpi04 mpi05 mpi06 mpi07 mpi08
	
